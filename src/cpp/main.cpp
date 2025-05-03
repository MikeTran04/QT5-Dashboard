#include <QApplication>
#include "dashboard.h"
#include <iostream>
#include <fstream>

#ifdef __arm__
#include <wiringPi.h>
#endif

/**
 * @class main
 * @brief The main entry point for the application.
 * @details main class contains the main function that checks if the program is running on a Raspberry Pi,
 *          sets up the GPIO pins if necessary, and launches the Qt application. It also initializes the `Dashboard`
 *          object, sets its window title, and display mode. The application enters the event loop after starting the dashboard.
 * 
 * @author Amaan Hafeez, Tianyi Han
 */

/**
 * @brief Checks if the application is running on a Raspberry Pi.
 * @details This function reads the `/proc/cpuinfo` file and searches for the strings "Raspberry Pi"
 *          or "Model" to determine if the program is running on a Raspberry Pi system.
 * @return True if the system is a Raspberry Pi, otherwise false.
 */
bool isRaspberryPi() {
    std::string line;
    std::ifstream cpuinfo("/proc/cpuinfo");  // Open the CPU info file
    if (cpuinfo.is_open()) {  // Check if the file was successfully opened
        while (getline(cpuinfo, line)) {  // Read each line from the file
            // Check if the line contains "Raspberry Pi" or "Model"
            if (line.find("Raspberry Pi") != std::string::npos || line.find("Model") != std::string::npos) {
                cpuinfo.close();  // Close the file
                return true;  // Return true if a match is found
            }
        }
        cpuinfo.close();  // Close the file if no match was found
    }
    return false;  // Return false if not running on a Raspberry Pi
}

/**
 * @brief Main entry point for the application.
 * @details This function checks if the program is running on a Raspberry Pi, sets up the GPIO pins if necessary,
 *          and launches the Qt application. It also initializes the `Dashboard` object and sets its window title
 *          and display mode. The program enters the event loop after starting the dashboard.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return The exit status of the application, returned by `app.exec()`.
 */
int main(int argc, char *argv[]) {
    bool onRaspberryPi = isRaspberryPi();  // Check if the program is running on a Raspberry Pi

#ifdef __arm__
    if (onRaspberryPi) {
        // Set up the wiringPi library for GPIO control on Raspberry Pi
        if (wiringPiSetup() == -1) {  // Initialize the wiringPi library
            return 1;  // Return 1 if wiringPi setup fails
        }
    } else {
        std::cout << "1st line not running on a Raspberry Pi. Using dummy sensor values." << std::endl;
    }
#else
    std::cout << "2nd line Not running on a Raspberry Pi. Using dummy sensor values." << std::endl;
#endif

    QApplication app(argc, argv);  // Initialize the Qt application
    Dashboard dashboard(onRaspberryPi);  // Create the dashboard object with Raspberry Pi flag
    dashboard.setWindowTitle("Pi Dashboard");  // Set the window title for the dashboard
    dashboard.showFullScreen();  // Show the dashboard in full-screen mode
    return app.exec();  // Enter the Qt application event loop
}
