#ifndef DATETIMEPAGE_H
#define DATETIMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>

class DateTimePage : public QWidget {
    Q_OBJECT

public:
    explicit DateTimePage(QWidget *parent = nullptr);
    ~DateTimePage();
    
    // Return individual labels for the stacked widget
    QLabel* getTimeLabel() const { return timeLabel; }
    QLabel* getDateLabel() const { return dateLabel; }
    QLabel* getGreetingLabel() const { return greetingLabel; }

public slots:
    void updateDateTime();  // Update date and time displays

private:
    // Declare the labels as member variables
    QLabel *timeLabel;       // Shows the current time
    QLabel *dateLabel;       // Shows the current date
    QLabel *greetingLabel;   // Shows a greeting message based on time of day
    
    QTimer *timer;           // Timer for periodic updates
};

#endif // DATETIMEPAGE_H
