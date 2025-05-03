#ifndef BRIGHTNESS_CONTROL_H
#define BRIGHTNESS_CONTROL_H

#include <QObject>

class Dashboard;  // Forward declaration

class BrightnessControl : public QObject {
    Q_OBJECT

public:
    explicit BrightnessControl(Dashboard *dashboard, QObject *parent = nullptr);
    void adjustBrightness(int ambientLightLevel);
    int getCurrentBrightness() const;

public slots:
    void updateBrightness(int brightness);  // new slot for real-time updates

signals:
    void brightnessChanged();

private:
    int currentBrightness;
    bool isActiveMode;
    Dashboard *dashBoard;
};

#endif 
