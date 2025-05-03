#include "hcsr501.h"
#include <QDebug>

/**
 * @class HCSR501
 * @brief This class provides an interface for the HCSR501 motion sensor.
 * 
 * The `HCSR501` class interacts with the HCSR501 sensor to detect motion and signal when motion is detected.
 * It can be used in environments like the Raspberry Pi, where it directly interacts with GPIO pins, 
 * or in simulated environments where motion detection is not available, and dummy values are returned.
 * 
 * @author Amaan Hafeez, Mike Tran
 */

/**
 * @brief Constructor for the HCSR501 class.
 * 
 * Initializes the HCSR501 sensor by setting up the GPIO pin on a Raspberry Pi (if applicable).
 * If running on a Raspberry Pi, the wiringPi library is initialized and the specified pin is set as an input.
 * 
 * @param pin The GPIO pin to which the HCSR501 sensor is connected.
 * @param onRaspberryPi A flag indicating whether the code is running on a Raspberry Pi. Default is true.
 * @param parent The parent QObject, which is passed to the base class constructor.
 */
HCSR501::HCSR501(int pin, bool onRaspberryPi, QObject *parent) : QObject(parent), pin(pin), m_onRaspberryPi(onRaspberryPi) {
#ifdef __arm__
    if (m_onRaspberryPi) {
        wiringPiSetup();
        pinMode(pin, INPUT);
    }
#endif
}

/**
 * @brief Destructor for the HCSR501 class.
 * 
 * The destructor can be used for any necessary cleanup. Currently, it is empty as no resources need to be freed.
 */
HCSR501::~HCSR501() {
    // Destructor if cleanup is needed
}

/**
 * @brief Reads the motion detection status from the sensor.
 * 
 * This method reads the current state of the HCSR501 motion sensor. If the system is not running on a Raspberry Pi, 
 * a dummy value (false) is returned. When running on a Raspberry Pi, it reads the state of the specified GPIO pin.
 * 
 * @return Returns true if motion is detected, false otherwise.
 * @note This function uses the wiringPi library when running on a Raspberry Pi.
 */
bool HCSR501::readMotion() {
    if (!m_onRaspberryPi) {
        return false; // Dummy value for non-Raspberry Pi environments
    }
#ifdef __arm__
    return digitalRead(pin);  // Read the state of the pin on a Raspberry Pi
#else
    return false;  // Return false in non-Raspberry Pi environments
#endif
}

/**
 * @brief Checks for motion detection and emits a signal.
 * 
 * This method checks if motion is detected by calling the `readMotion` function and emits a signal indicating 
 * whether motion was detected. The `motionDetected` signal is emitted with the status of the motion detection.
 * 
 * @note This method can be used in conjunction with Qt's signal-slot mechanism to handle motion detection events.
 */
void HCSR501::checkMotion() {
    bool detected = readMotion();
    emit motionDetected(detected);  // Emit signal with motion detection status
}
