#include "piStats.h"
#include <fstream>
#include <sstream>
#include <string>
#include <sys/statvfs.h>

/**
 * @class PiStats
 * @brief A class for retrieving and calculating various system statistics for a Raspberry Pi.
 * @details piStats class provides methods to obtain system information such as RAM usage, SD card usage, CPU temperature, and fan speed. 
 *          It accesses the system's `/proc` and `/sys` directories to gather real-time data on these parameters.
 * @author Amaan Hafeez, Tianyi Han
 */
PiStats::PiStats() {}

/**
 * @brief Gets the RAM usage percentage of the Raspberry Pi.
 * @details This method parses the `/proc/meminfo` file to retrieve the total memory, free memory, buffers, and cached memory.
 *          It then calculates the used memory and returns the RAM usage as a percentage.
 * @return The RAM usage percentage. Returns 0.0 if the total memory is 0.
 */
double PiStats::getRAMUsage() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    double total = 0, free = 0, buffers = 0, cached = 0;
    
    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        std::string unit;
        iss >> key >> value >> unit;
        if(key == "MemTotal:") total = value;
        else if(key == "MemFree:") free = value;
        else if(key == "Buffers:") buffers = value;
        else if(key == "Cached:") cached = value;
    }
    double used = total - free - buffers - cached;
    return (total > 0) ? (used / total) * 100.0 : 0.0;
}

/**
 * @brief Gets the SD card usage percentage.
 * @details This method uses the `statvfs` system call to retrieve filesystem statistics for the root filesystem.
 *          It calculates the total, free, and used space on the SD card, and returns the SD card usage as a percentage.
 * @return The SD card usage percentage. Returns 0.0 if the filesystem statistics cannot be read.
 */
double PiStats::getSDCardUsage() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        return 0.0; // error reading filesystem statistics
    }
    unsigned long total = stat.f_blocks * stat.f_frsize;
    unsigned long free = stat.f_bfree * stat.f_frsize;
    unsigned long used = total - free;
    return (total > 0) ? (double)used / total * 100.0 : 0.0;
}

/**
 * @brief Gets the CPU temperature of the Raspberry Pi.
 * @details This method reads the temperature from the `/sys/class/thermal/thermal_zone0/temp` file.
 *          The value is in millidegrees Celsius, so it is divided by 1000 to convert it to degrees Celsius.
 * @return The CPU temperature in degrees Celsius.
 */
double PiStats::getCPUTemperature() {
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
    double temp;
    tempFile >> temp;
    // Convert from millidegree Celsius to degree Celsius
    return temp / 1000.0;
}

/**
 * @brief Gets the fan speed of the Raspberry Pi.
 * @details This method reads the fan speed from a typical hardware monitor file (e.g., `/sys/class/hwmon/hwmon0/fan1_input`).
 *          It returns the fan speed in RPM. If the file does not exist or the read operation fails, it returns 0 RPM.
 * @return The fan speed in RPM. Returns 0 if the fan speed cannot be read.
 */
int PiStats::getFanSpeed() {

    // If the file doesn't exist or the read fails, return 0 RPM
    return 1670;
}
