#ifndef PISTATS_H
#define PISTATS_H

#include <QString>

class PiStats {
public:
    PiStats();
    // Returns the RAM usage as a percentage (0.0 - 100.0)
    double getRAMUsage();
    // Returns the SD card usage as a percentage (0.0 - 100.0)
    double getSDCardUsage();
    // Returns the CPU temperature in Celsius
    double getCPUTemperature();
    // Returns the fan speed in RPM (if available; otherwise returns 0)
    int getFanSpeed();
};

#endif // PISTATS_H
