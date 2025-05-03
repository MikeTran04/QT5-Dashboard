#ifndef HCSR501_H
#define HCSR501_H

#include <QObject>

#ifdef __arm__
#include <wiringPi.h>
#endif

class HCSR501 : public QObject {
    Q_OBJECT

public:
    explicit HCSR501(int pin, bool onRaspberryPi, QObject *parent = nullptr);
    ~HCSR501();
    
    bool readMotion();  // Read motion status (true = detected, false = no motion)

signals:
    void motionDetected(bool detected);

public slots:
    void checkMotion();  // Slot to periodically check for motion

private:
    int pin;
    bool m_onRaspberryPi; // Add this line
};

#endif // HCSR501_H
