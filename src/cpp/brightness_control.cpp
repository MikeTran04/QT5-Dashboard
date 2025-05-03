#include "brightness_control.h"
#include "dashboard.h"
#include <QGraphicsOpacityEffect>
#include <QDebug>

/**
 * @class BrightnessControl
 * @brief Controls the screen brightness based on ambient light levels.
 * @details This class monitors the ambient light level and adjusts the screen brightness accordingly.
 *          It emits a signal when the brightness is changed. It also provides methods to get and
 *          manually update the current brightness.
 * 
 * @author Mike Tran
 */
BrightnessControl::BrightnessControl(Dashboard *dashboard, QObject *parent)
    : QObject(parent), currentBrightness(100), isActiveMode(true), dashBoard(dashboard) {}

/**
 * @brief Adjusts the brightness based on the ambient light level.
 * @details This function takes the ambient light level as input, scales it to a value between 0 and 100,
 *          and updates the current brightness accordingly. If the new brightness is different from the
 *          current one, it updates the brightness and emits a signal.
 * 
 * @param ambientLightLevel The level of ambient light, used to determine the new brightness.
 */
void BrightnessControl::adjustBrightness(int ambientLightLevel) {
    int newBrightness = qBound(0, ambientLightLevel / 10, 100);  // Scale and constrain brightness
    qDebug() << "New brightness: " << newBrightness;  // Log new brightness value
    if (newBrightness != currentBrightness) {
        currentBrightness = newBrightness;  // Update current brightness if changed
        qDebug() << "Adjusted brightness to:" << currentBrightness;  // Debug log for the adjusted brightness
        emit brightnessChanged();  // Emit signal indicating brightness change
    }
}

/**
 * @brief Returns the current brightness level.
 * @details This function returns the current brightness value.
 * 
 * @return The current brightness level (0-100).
 */
int BrightnessControl::getCurrentBrightness() const {
    return currentBrightness;  // Return the current brightness value
}

/**
 * @brief Updates the brightness by directly adjusting it to the provided value.
 * @details This function allows manual updates to the brightness. It calls the `adjustBrightness` function
 *          with the provided value.
 * 
 * @param brightness The new brightness level to be set.
 */
void BrightnessControl::updateBrightness(int brightness) {
    adjustBrightness(brightness);  // Adjust the brightness to the provided value
}
