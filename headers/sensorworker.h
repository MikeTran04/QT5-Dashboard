#pragma once

#include <QObject>
#include "dht22lib.h"  // Include the DHT22 class

class SensorWorker : public QObject
{
    Q_OBJECT

public:
    explicit SensorWorker(int pin, bool onRaspberryPi, QObject *parent = nullptr)
        : QObject(parent), m_sensor(pin, onRaspberryPi) {}

public slots:
    void doWork();  // Declaration only

signals:
    void sensorDataReady(float temperature, float humidity);  // Declare the signal

private:
    DhtSensor m_sensor;  // DHT22 sensor object
};
