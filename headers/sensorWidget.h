#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class SensorWidget : public QWidget {
    Q_OBJECT

public:
    explicit SensorWidget(QWidget *parent = nullptr);
    ~SensorWidget();

public slots:
    void updateSensorData(float temperature, float humidity);  // Updates temperature and humidity displays

private:
    QLabel *tempLabel;       // Shows temperature reading
    QLabel *humidityLabel;   // Shows humidity reading
};

#endif // SENSORWIDGET_H