#ifndef MOTIONSENSORPAGE_H
#define MOTIONSENSORPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MotionSensorPage : public QWidget {
    Q_OBJECT

public:
    explicit MotionSensorPage(QWidget *parent = nullptr);
    ~MotionSensorPage();
    
    QLabel* getMotionLabel() const { return motionLabel; }

public slots:
    void updateMotionStatus(bool detected);

private:
    QLabel *motionLabel;
};

#endif 