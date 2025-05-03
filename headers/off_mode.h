#ifndef OFF_MODE_H
#define OFF_MODE_H

#include <QObject>
#include "brightness_control.h"

// OffMode class to manage the Off mode behavior of the dashboard
class OffMode : public QObject {
    Q_OBJECT

public:
    // Constructor to initialize OffMode with a BrightnessControl and Dashboard instance
    explicit OffMode(BrightnessControl *brightnessControl, Dashboard* dashboard, QObject *parent = nullptr);

    bool getIsOff() const;
    // Method to enter OffMode (turn off the screen or related actions)
    void enterOffMode();

    // Method to exit OffMode and revert to previous state
    void exitOffMode();

    // Monitor the proximity sensor data to adjust behavior in OffMode
    void monitorProximity(double range);

signals:
    // Signal to notify when the mode button changes (ActiveMode or OffMode)
    void buttonChanged(bool isActive);

    // Signal to notify when the mode changes (ActiveMode or OffMode)
    void modeChanged(bool isActive);

private:
    bool isOff;  // Flag to track if the system is currently in OffMode
    Dashboard *dashboard;  // Pointer to the Dashboard instance
    BrightnessControl *brightnessControl;  // Pointer to the BrightnessControl instance
};

#endif // OFF_MODE_H
