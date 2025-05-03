#include "sensorworker.h"

/**
 * @class SensorWorker
 * @brief Worker class to handle sensor data reading.
 * @details SensorWorker class is responsible for reading data from a sensor and emitting the data to other components of the system. 
 *          It utilizes a member `m_sensor` which provides the raw sensor readings for temperature and humidity. 
 * 
 * @author Amaan Hafeez, Tianyi Han
 */

/**
 * @brief Reads sensor data and emits the results.
 * @details This method reads temperature and humidity values from the sensor, then emits the results as a signal. 
 *          The signal contains the temperature and humidity data, which can be connected to other components to display or process the data.
 */
void SensorWorker::doWork() {
    m_sensor.read();  // Read from the sensor

    // Emit the sensor data (temperature and humidity) after reading from the sensor
    emit sensorDataReady(m_sensor.m_temperature, m_sensor.m_humidity);
}
