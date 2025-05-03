#include "off_mode.h"
#include "dashboard.h"
#include <QDebug>

const int OFF_BRIGHTNESS = 0;  // Brightness level when in OffMode
const int FULL_BRIGHTNESS = 1000;  // Full brightness level for ActiveMode




/**
 * @class OffMode
 * @brief A class that handles the OffMode state of the Raspberry Pi dashboard.
 * @details OffMode class is responsible for managing the system's behavior when in OffMode, 
 *          including adjusting brightness, disabling widgets, and monitoring the proximity sensor to 
 *          transition to ActiveMode. The class interacts with the BrightnessControl and Dashboard objects.
 * @author Mike Tran
 */

/**
 * @brief Constructs an OffMode object.
 * @details This constructor initializes the OffMode with instances of BrightnessControl and Dashboard.
 *          It also sets the initial state of OffMode to "off" and initializes other member variables.
 * @param brightnessControl A pointer to the BrightnessControl instance used for adjusting brightness.
 * @param dashboard A pointer to the Dashboard instance, used to interact with various widgets.
 * @param parent The parent QObject (optional, default is nullptr).
 */
OffMode::OffMode(BrightnessControl *brightnessControl, Dashboard* dashboard, QObject *parent)
    : QObject(parent), isOff(false), dashboard(dashboard), brightnessControl(brightnessControl) {
    //qDebug() << "OffMode initialized.";  // Debugging initialization
}

/**
 * @brief Gets the current status of OffMode.
 * @details Returns whether OffMode is currently active.
 * @return A boolean indicating whether OffMode is active.
 */
bool OffMode::getIsOff() const {
    return isOff;
}

/**
 * @brief Enters OffMode.
 * @details This method sets the system to OffMode
 *          Setting the brightness to OFF_BRIGHTNESS.
 *          Disabling interactions with certain dashboard widgets.
 *          Emitting the modeChanged signal to notify that OffMode has been entered.
 *          Logging the action for debugging purposes.
 */
void OffMode::enterOffMode() {
    if (!isOff) {  // Only enter OffMode if not already in it
        isOff = true;  // Set flag to indicate OffMode is active
        // Set brightness to OFF_BRIGHTNESS
        brightnessControl->adjustBrightness(OFF_BRIGHTNESS);
        
        dashboard->getWeatherStation()->setEnabled(false);  // Disables interaction with the weather station widget
        dashboard->getYouTubeMusicPage()->setEnabled(false);  // Disables YouTube music widget
        dashboard->getYouTubePage()->setEnabled(false);  // Disables YouTube widget
        dashboard->getDateTimePage()->setEnabled(false);
        dashboard->getCalendarPage()->setEnabled(false);

        emit modeChanged(false);    // Notify that the button is in OffMode
        qDebug() << "Entered OffMode.";
    }
}

/**
 * @brief Exits OffMode and enters ActiveMode.
 * @details This method exits OffMode and transitions the system to ActiveMode.
 *          It sets the `isOff` flag to false and emits the modeChanged signal to notify that ActiveMode has been entered.
 *          It also logs the action for debugging purposes.
 */
void OffMode::exitOffMode() {
    if (isOff) {  // Only exit OffMode if currently in OffMode
        isOff = false;  // Set flag to indicate OffMode has been exited
        emit modeChanged(true);  // Notify that ActiveMode is now entered
        qDebug() << "Exited OffMode.";
    }
}

/**
 * @brief Monitors proximity sensor data and handles transitions between OffMode and ActiveMode.
 * @details This method checks the range value provided by the proximity sensor. If the range is less than 10,
 *          it indicates that the user is near, and the system will exit OffMode and enter ActiveMode.
 * @param range The proximity sensor's range value, indicating the distance to the object detected.
 */
void OffMode::monitorProximity(double range) {
    //qDebug() << "Proximity status changed in OffMode. In range: " << inRange;
    //qDebug() << "isOff?" << isOff;

    // If in OffMode and proximity sensor indicates in range, switch to ActiveMode
    if (range < 10) {
        qDebug() << "Proximity in range. Exiting OffMode and entering ActiveMode.";
        exitOffMode();  // Exit OffMode and switch to ActiveMode
    }
}
