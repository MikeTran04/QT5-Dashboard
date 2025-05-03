/**
 * @file dht22lib.h
 * @brief This file provides the implementation of DHT22 sensor reading, decoding algorithms, and sensor data handling.
 * 
 * The file defines two decoder algorithms (twoMeansDecoder and splitDecoder) for interpreting signals from the DHT22 sensor. 
 * It also provides the implementation of the DhtSensor class, which handles sensor initialization, data reading, and decoding. 
 * The class interacts with the hardware to fetch temperature and humidity data, handling success and failure cases.
 * 
 * @author Amaan Hafeez, Maotong Wei
 */

#include "dht22lib.h"

#define DEFAULT_TEXT printf("\033[0m");

/**
 * @brief Decodes sensor data using the Two Means Decoding algorithm.
 * 
 * This algorithm assigns sensor data points to two centroids (lower and upper) based on their proximity, 
 * iteratively recalculating the centroids until convergence is achieved. It classifies data points 
 * into two clusters based on their proximity to the centroids and updates the binary assignment of each bit.
 * 
 * @param x The sensor data array containing the raw signal data.
 * @param binaryAssignment The binary array to store the decoded binary values based on the centroids.
 * 
 * @details The Two Means Decoding algorithm works by initializing the lower and upper centroids as the minimum 
 * and maximum values of the sensor data, then iteratively assigning each data point to the nearest centroid 
 * and updating the centroids based on the data points assigned to them. The algorithm stops when the binary 
 * assignments no longer change (i.e., convergence).
 */
void twoMeansDecoder(const int (&x)[NBITS], bool (&binaryAssignment)[NBITS]) {
    // Initialize centroids as the minimum and maximum values
    float lower = x[0];
    float upper = x[0];
    for (int elem : x) {
        if (elem < lower) lower = elem;
        if (elem > upper) upper = elem;
    }

    // Initialize binary assignments
    for (bool &elem : binaryAssignment)
        elem = false;

    while (true) {
        // Assignment step: assign each observation to the nearest centroid
        bool newAssignUpper[NBITS];
        for (int j = 0; j < NBITS; j++) {
            if ((abs(lower - x[j]) > abs(upper - x[j])))
                newAssignUpper[j] = true;
            else
                newAssignUpper[j] = false;
        }

        // Update step: update centroid locations
        float newLower = 0, newUpper = 0;
        int numUpperObservations = 0;
        for (int j = 0; j < NBITS; j++) {
            numUpperObservations += newAssignUpper[j];
            if (newAssignUpper[j]) 
                newUpper += x[j];
            else 
                newLower += x[j];
        }
        newUpper /= numUpperObservations;
        newLower /= (NBITS - numUpperObservations);

        // Check for convergence
        bool converged = true;
        for (int j = 0; j < NBITS; j++) {
            if (newAssignUpper[j] != binaryAssignment[j]) {
                converged = false;
                break;
            }
        }
        if (converged) return;

        // Update centroids and assignments
        upper = newUpper;
        lower = newLower;
        for (int j = 0; j < NBITS; j++)
            binaryAssignment[j] = newAssignUpper[j];
    }
}

/**
 * @brief Decodes sensor data using the Split Decoder algorithm.
 * 
 * The Split Decoder algorithm classifies the sensor data by comparing each value to the midpoint 
 * between the minimum and maximum values in the data set. Values greater than the midpoint are assigned to a 
 * binary value of 1, and values less than or equal to the midpoint are assigned to 0.
 * 
 * @param x The sensor data array containing the raw signal data.
 * @param binaryAssignment The binary array to store the decoded binary values based on the midpoint comparison.
 * 
 * @details The Split Decoder works by calculating the midpoint between the minimum and maximum values 
 * of the data array and assigning each data point a binary value (0 or 1) based on whether it is greater or 
 * less than the midpoint.
 */
void splitDecoder(const int (&x)[NBITS], bool (&binaryAssignment)[NBITS]) {
    // Calculate the midpoint between the min and max values
    int min = x[0];
    int max = x[0];
    for (int elem : x) {
        if (elem < min) min = elem;
        if (elem > max) max = elem;
    }

    int midpoint = min + (max - min) / 2;

    // Classify each value based on the midpoint
    for (int j = 0; j < NBITS; j++)
        binaryAssignment[j] = (x[j] > midpoint);
}

/**
 * @class DhtSensor
 * @brief This class handles interactions with the DHT22 sensor, including data reading and decoding.
 * 
 * The DhtSensor class is responsible for managing the DHT22 sensor, reading its data, and decoding 
 * the signals into temperature and humidity values. It uses different decoding algorithms to interpret the 
 * signal and handles error states, including checksum validation.
 * 
 * @author Mike Tran
 */

/**
 * @brief Constructor for the DhtSensor class.
 * 
 * Initializes the DHT sensor with the specified pin and whether the code is running on a Raspberry Pi.
 * 
 * @param pin The GPIO pin to which the DHT22 sensor is connected.
 * @param onRaspberryPi A flag indicating if the code is running on a Raspberry Pi (default is true).
 */
DhtSensor::DhtSensor(int pin, bool onRaspberryPi) : m_pin{pin}, m_onRaspberryPi{onRaspberryPi} {}

/**
 * @brief Reads data from the DHT22 sensor and decodes the signal.
 * 
 * This function initiates a read from the DHT22 sensor, processes the signal, and stores the resulting 
 * temperature and humidity values. It also sets the read type status based on whether the reading was 
 * successful or not. If the code is not running on a Raspberry Pi, it returns default dummy values.
 * 
 * @details If running on a Raspberry Pi, the sensor data is fetched through GPIO pin reads. The raw data is 
 * then decoded using the twoMeansDecoder algorithm, and a checksum is validated. If the checksum is correct, 
 * the temperature and humidity values are updated, otherwise an error state is set.
 */
void DhtSensor::read() {
    if (!m_onRaspberryPi) {
        // Use dummy values if not on Raspberry Pi
        m_humidity = 50.0;
        m_temperature = 25.0;
        m_readType = GOOD;
        return;
    }

#ifdef __arm__
    uint8_t lastState = HIGH;
    uint8_t stateDuration = 0;
    uint8_t stateChanges = 0;
    uint8_t bitsRead = 0;
    float humidity = BAD_VALUE;
    float temperature = BAD_VALUE;

    int data[5] = {0, 0, 0, 0, 0};
    for (int &elem : m_signalStateDurations)
        elem = BAD_VALUE;

    // Signal the sensor to prepare for reading
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
    delay(10);
    digitalWrite(m_pin, LOW);
    delay(18);
    digitalWrite(m_pin, HIGH);
    delayMicroseconds(40);

    pinMode(m_pin, INPUT);

    // Read data from the sensor pin
    for ((stateChanges = 0), (stateDuration = 0); 
         (stateChanges < MAX_TIMINGS) && (stateDuration < 255) && (bitsRead < NBITS); 
         stateChanges++) {
        stateDuration = 0;
        while ((digitalRead(m_pin) == lastState) && (stateDuration < 255)) {
            stateDuration++;
            delayMicroseconds(1);
        }

        lastState = digitalRead(m_pin);

        if ((stateChanges > 2) && (stateChanges % 2 == 0)) {
            m_signalStateDurations[bitsRead] = stateDuration;
            bitsRead++;
        }
    }

    // Ensure all 40 bits were read
    for (int elem : m_signalStateDurations) {
        if (elem == BAD_VALUE) {
            m_humidity = BAD_VALUE;
            m_temperature = BAD_VALUE;
            m_readType = ERROR;
            return;
        }
    }

    // Decode the data
    twoMeansDecoder(m_signalStateDurations, m_signalData);

    for (int j = 0; j < NBITS; j++) {
        data[j / 8] <<= 1;    // Shift bits left
        if (m_signalData[j])  // Set bit if it's a 1
            data[j / 8] |= 0x00000001;
    }

    // Checksum validation
    if ((bitsRead >= NBITS) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        humidity = (float)((data[0] << 8) + data[1]) / 10.0;
        temperature = (float)((data[2] << 8) + data[3]) / 10.0;
        if (data[2] & 0x80)  // Handle negative temperature
            temperature *= -1;
        m_readType = GOOD;
    } else {
        m_readType = BAD;
    }

    // Update member variables
    m_humidity = humidity;
    m_temperature = temperature;
#endif
}
