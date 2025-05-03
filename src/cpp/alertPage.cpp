#include "alertPage.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDateTime>

/**
 * @class AlertPage
 * @brief Displays and manages alert notifications and logs on the user interface.
 * @details This class creates a page to display alerts, trigger a test alert, 
 *          dismiss the alert, and keep a log of all triggered alerts with timestamps.
 *          It uses buttons to trigger and dismiss alerts, and a list widget to record alert history.
 * 
 * @author Maotong Wei
 */
AlertPage::AlertPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // USED TO DISPLAY THE CURRENT ALERT INFORMATION
    alertLabel = new QLabel("No alerts.", this);  
    alertLabel->setAlignment(Qt::AlignCenter);  
    alertLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: red;");  // Style for alert text
    layout->addWidget(alertLabel);
    
    // BUTTON TO TRIGGER A TEST ALERT
    QString buttonStyle = "font-size: 16px; padding: 8px; "
                            "font-weight: bold; "   // Blue background
                            "background-color:rgb(63, 180, 100); " 
                            "color: white; "                 // White text
                            "border: 5px solidrgb(84, 0, 158); ";    // Slight border for contrast

    triggerButton = new QPushButton("Trigger Test Alert", this);  
    triggerButton->setStyleSheet(buttonStyle);  
    layout->addWidget(triggerButton);
    
    // BUTTON TO DISMISS THE ALERT / RETURN
    dismissButton = new QPushButton("Dismiss Alert / Back", this);
    dismissButton->setStyleSheet(buttonStyle);
    layout->addWidget(dismissButton);
    
    // ALERT LOG TO RECORD ALERT HISTORY
    alertLog = new QListWidget(this);  // List widget to display past alerts
    layout->addWidget(alertLog);
    
    setLayout(layout);
    
    // CONNECT BUTTON SIGNALS
    connect(triggerButton, &QPushButton::clicked, this, &AlertPage::triggerTestAlert);  // Trigger test alert
    connect(dismissButton, &QPushButton::clicked, this, &AlertPage::dismissAlert);      // Dismiss the alert and return
}

/**
 * @brief Triggers a test alert with a predefined message.
 * @details This function simulates a severe storm warning alert by updating 
 *          the alert label with the corresponding message and adding the alert 
 *          record with a timestamp to the alert log.
 */
void AlertPage::triggerTestAlert() {
    // SIMULATE AN ALERT, FOR EXAMPLE A SEVERE STORM WARNING
    QString alertText = "Severe storm warning! Take cover immediately!";
    alertLabel->setText(alertText);  // Update the alert label with the new alert message
    
    // ADD THE ALERT RECORD WITH A TIMESTAMP TO THE ALERT LOG
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");  // Get current timestamp
    alertLog->addItem(timeStamp + " - " + alertText);  // Add the alert to the log with timestamp
}

/**
 * @brief Dismisses the current alert and returns to the main page.
 * @details This function clears the current alert message and emits a signal 
 *          to indicate that the user has requested to return to the main page.
 */
void AlertPage::dismissAlert() {
    // CLEAR THE CURRENT ALERT DISPLAY
    alertLabel->setText("No alerts.");  // Reset the alert label to indicate no current alerts
    
    // EMIT SIGNAL TO RETURN TO THE MAIN PAGE
    emit backRequested();  // Signal emitted to return to the main page
}

