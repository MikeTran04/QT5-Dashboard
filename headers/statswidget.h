// statswidget.h
#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QFrame>
#include <QDebug>
#include "piStats.h"

class StatsWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatsWidget(QWidget *parent = nullptr);
    ~StatsWidget();
    
    void setPiStats(PiStats* stats) { piStats = stats; }

public slots:
    void updateStats();  // Update displayed statistics

private:
    void createLabels();
    void setupLayout();
    
    QFrame* statsFrame;
    QLabel* titleLabel;
    QLabel* ramLabel;
    QLabel* sdCardLabel;
    QLabel* cpuTempLabel;
    QLabel* fanSpeedLabel;
    
    QTimer* updateTimer;
    PiStats* piStats;
};

#endif // STATSWIDGET_H