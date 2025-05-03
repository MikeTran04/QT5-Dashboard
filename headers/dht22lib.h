#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <csignal>
#include <iostream>

#ifdef __arm__
#include <wiringPi.h>
#endif

#define MAX_TIMINGS 85  // Takes 84 state changes to transmit data
#define NBITS 40        // Total number of bits of data
#define BAD_VALUE 999

// Function declarations
void twoMeansDecoder(const int (&x)[NBITS], bool (&binaryAssignment)[NBITS]);
void splitDecoder(const int (&x)[NBITS], bool (&binaryAssignment)[NBITS]);

enum DhtReadType {
    GOOD,   // Data is read and checksum passes
    BAD,    // Data is read and checksum fails
    ERROR,  // Attempt to read data, but failed
    NONE,   // No attempt to read data
};

class DhtSensor {
public:
    int m_pin;
    float m_humidity{BAD_VALUE};
    float m_temperature{BAD_VALUE};
    DhtReadType m_readType{NONE};
    bool m_onRaspberryPi;

    int m_signalStateDurations[NBITS];
    bool m_signalData[NBITS];

    DhtSensor(int pin, bool onRaspberryPi);  // Constructor
    void read();         // Read sensor data

};
