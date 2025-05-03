#include "active_mode.h"
#include "dashboard.h"
#include <QTimer>
#include <QDebug>

// Constants for brightness levels
const int FULL_BRIGHTNESS = 1000;  // Full brightness level (1000)
const int LOW_BRIGHTNESS = 100;    // Low brightness level (100)
const int OFF_BRIGHTNESS = 0;      // Off brightness level (0)

/**
 * @class ActiveMode
 * @brief Manages the active mode of the dashboard, adjusting brightness based on proximity.
 * @details This class handles the active mode functionality of the dashboard. It includes 
 *          controlling screen brightness, entering and exiting active mode, and monitoring 
 *          proximity status for screen adjustments. It uses a timer to monitor proximity 
 *          and adjust the system behavior accordingly.
 * 
 * @author Mike Tran
 */
ActiveMode::ActiveMode(BrightnessControl *brightnessControl, Dashboard* dashboard, QObject *parent)
        : QObject(parent), 
        sleepStatus(false),                   
        brightnessControl(brightnessControl), 
        dashboard(dashboard),                
        isActive(true),                      
        sleepTimer(new QTimer(this))           // Timer for proximity checks
{
    // Initialize proximity timer with 5-second interval for proximity detection
    sleepTimer = new QTimer(this);
    sleepTimer->setInterval(5000);  

    // Connect timer's timeout to handle proximity out-of-range behavior
    connect(sleepTimer, &QTimer::timeout, this, [=]() {
        // Check if proximity is out of range and adjust brightness
        qDebug() << "sleepStatus: " << sleepStatus;
        qDebug() << "isActive: " << isActive;
        if (sleepStatus && isActive) {
            brightnessControl->adjustBrightness(LOW_BRIGHTNESS);  
            
            // Exit active mode after 5 seconds if proximity remains out of range
            QTimer::singleShot(5000, this, [=]() {
                if (sleepStatus) exitActiveMode();
            });
        }
    });
}

/**
 * @brief Returns the current sleep status of the system.
 * @details This function returns whether the system is in sleep mode or not.
 * @return A boolean indicating the sleep status (true for sleep mode, false for active mode).
 */
bool ActiveMode::getSleepStatus() const {
    return sleepStatus;
}

/**
 * @brief Sets the sleep status of the system.
 * @details This function updates the sleep status. If the status is true, the system enters 
 *          sleep mode; if false, it enters active mode.
 * @param status A boolean indicating the new sleep status.
 */
void ActiveMode::setSleepStatus(bool status) {
    sleepStatus = status;
}

/**
 * @brief Returns the current active status of the system.
 * @details This function returns whether the system is in active mode or not.
 * @return A boolean indicating the active status (true for active mode, false for sleep mode).
 */
bool ActiveMode::getIsActive() const {
    return isActive;
}
/**
 * @brief Enters active mode and adjusts the system's brightness and widget accessibility.
 * @details This function enters the active mode, adjusting brightness to full and enabling 
 *          interaction with the dashboard widgets.
 */
void ActiveMode::enterActiveMode() {
    // Enter ActiveMode if not already active
    if (!isActive) {  
        isActive = true;
        brightnessControl->adjustBrightness(FULL_BRIGHTNESS);  // Set brightness to full
        
        // Re-enable interaction with widgets
        dashboard->getWeatherStation()->setEnabled(true);  
        dashboard->getYouTubeMusicPage()->setEnabled(true);
        dashboard->getYouTubePage()->setEnabled(true);
        dashboard->getDateTimePage()->setEnabled(true);
        dashboard->getCalendarPage()->setEnabled(true);
        
        emit modeChanged(true);  // Notify that the mode has changed to active
        qDebug() << "Entered OnMode";
    }
}

/**
 * @brief Exits active mode and turns off the system's brightness.
 * @details This function exits the active mode and adjusts the brightness to the off level.
 */
void ActiveMode::exitActiveMode() {
    // Exit ActiveMode and adjust brightness to off
    if (isActive) {  
        isActive = false;
        emit modeChanged(false);  // Notify that the mode has changed to inactive
        qDebug() << "Exited OnMode";
    } 
}

/**
 * @brief Monitors proximity and adjusts the system's behavior based on the proximity range.
 * @details This function checks the proximity range and adjusts the system's sleep status and 
 *          brightness accordingly. If the proximity is within range, the system remains in active mode.
 *          If out of range, the system enters sleep mode after a 5-second timer.
 * @param range The current proximity range.
 */
void ActiveMode::monitorProximity(double range) {
    // Update proximity status and adjust behavior based on in-range status

    if (range < 10) {
        // Stop timer and ensure full brightness when proximity is in range
        if (sleepTimer->isActive()) sleepTimer->stop();
        enterActiveMode();  // Ensure the system remains in active mode
    } else {
        // Start proximity timer if it's not already active
        if (!sleepTimer->isActive()) {  
            sleepStatus = true;
            sleepTimer->start();     // Start the timer for proximity monitoring
            qDebug() << "Proximity out of range. Timer started.";
        }
    }
}
