#ifndef ACTIVE_MODE_H
#define ACTIVE_MODE_H

#include <QObject>
#include <QTimer>
#include "brightness_control.h"

class ActiveMode : public QObject {
    Q_OBJECT

public:
    explicit ActiveMode(BrightnessControl *brightnessControl, Dashboard* dashboard, QObject *parent = nullptr);
    bool getSleepStatus() const;
    void setSleepStatus(bool status);
    bool getIsActive() const;
    void enterActiveMode();  // Activates the ActiveMode and sets brightness to full
    void exitActiveMode();   // Deactivates ActiveMode and turns off brightness
    void monitorProximity(double range);  // Monitors proximity status
      // Tracks proximity status (in or out of range)

signals:
    void buttonChanged(bool isActive);  // Emitted when the button state changes
    void modeChanged(bool isActive);  // Emitted when the mode changes

private:
bool sleepStatus;
    BrightnessControl *brightnessControl;  // Manages brightness control
    Dashboard* dashboard;  // Reference to the dashboard
    bool isActive;  // Indicates if ActiveMode is active
    QTimer *sleepTimer;  // Timer for proximity detection
    
    
};

#endif 
