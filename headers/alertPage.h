#ifndef ALERTPAGE_H
#define ALERTPAGE_H

#include <QWidget>

class QPushButton;
class QLabel;
class QListWidget;

class AlertPage : public QWidget {
    Q_OBJECT
public:
    explicit AlertPage(QWidget *parent = nullptr);

signals:
    // WHEN THE USER CLICKS TO DISMISS THE ALERT (OR THE BACK BUTTON), THIS SIGNAL IS EMITTED TO RETURN TO THE MAIN PAGE
    void backRequested();

public slots:
    // SIMULATE TRIGGERING AN ALERT (FOR TESTING PURPOSES)
    void triggerTestAlert();

private slots:
    void dismissAlert();

private:
    QLabel *alertLabel; // displays the current information
    QPushButton *triggerButton; // used to trigger a test alert 
    QPushButton *dismissButton; // used to dismiss the alert
    QListWidget *alertLog; // displays alert history 
};

#endif 

