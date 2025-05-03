#include "dateTimePage.h"
#include <QVBoxLayout>
#include <QPalette>
#include <QPixmap>
#include <QPropertyAnimation>

/**
 * @class DateTimePage
 * @brief A widget that displays the current date, time, and a greeting message.
 * @details This class creates a simple date-time page that shows the current time,
 *          date, and a dynamic greeting based on the time of day. It includes 
 *          an animated clock effect that fades in and out every second.
 *          The page is updated once per second using a QTimer.
 * @author Nael Pervez Lakhani, Amaan Hafeez
 */
DateTimePage::DateTimePage(QWidget *parent) : QWidget(parent) {
    // Initialize labels for displaying time, date, and greeting
    timeLabel = new QLabel(this); // Label to display current time
    dateLabel = new QLabel(this); // Label to display current date
    greetingLabel = new QLabel(this); // Label to display greeting message

    // Set up time label appearance
    timeLabel->setAlignment(Qt::AlignCenter);  // Align text to the center.
    timeLabel->setStyleSheet("font-size: 90px; color: #E0E0E0; font-weight: bold; text-shadow: 2px 2px 10px rgba(255, 255, 255, 0.3);");
    
    // Set up date label appearance
    dateLabel->setAlignment(Qt::AlignCenter);  // Align text to the center.
    dateLabel->setStyleSheet("font-size: 45px; color: #A0A0A0; text-shadow: 2px 2px 8px rgba(200, 200, 200, 0.5);");
    
    // Set up greeting label appearance
    greetingLabel->setAlignment(Qt::AlignCenter);  // Align text to the center.
    greetingLabel->setStyleSheet("font-size: 50px; color: #FFD700; font-weight: bold; text-shadow: 2px 2px 10px rgba(255, 215, 0, 0.5);");

    // Layout setup to organize the labels on the page
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(); // Adds spacing above the labels.
    layout->addWidget(greetingLabel);  // Add the greeting label.
    layout->addWidget(timeLabel);  // Add the time label.
    layout->addWidget(dateLabel);  // Add the date label.
    layout->addStretch(); // Adds spacing below the labels.
    setLayout(layout);

    // Create and start a timer for updating the date/time
    timer = new QTimer(this); // Timer for updating time and date every second
    connect(timer, &QTimer::timeout, this, &DateTimePage::updateDateTime); // Connect timeout signal to updateDateTime slot
    timer->start(1000); // Start the timer to update every second
    
    // Initial update of the date and time display
    updateDateTime();
}

/**
 * @brief Destructor for the DateTimePage class.
 * @details Stops the timer if it is still active to ensure proper cleanup.
 */
DateTimePage::~DateTimePage() {
    if (timer->isActive()) {  // Check if the timer is running.
        timer->stop();  // Stop the timer.
    }
}

/**
 * @brief Updates the displayed time, date, and greeting message.
 * @details This function retrieves the current date and time, updates the
 *          timeLabel and dateLabel, and sets the appropriate greeting message
 *          based on the time of day (Morning, Afternoon, Evening).
 *          Additionally, it animates the clock's opacity with a fade-in effect.
 */
void DateTimePage::updateDateTime() {
    QDateTime now = QDateTime::currentDateTime();  // Get current date and time

    // Update the time label with current time in HH:mm:ss format
    timeLabel->setText(now.toString("HH:mm:ss"));

    // Update the date label with current date in "day, month day, year" format
    dateLabel->setText(now.toString("dddd, MMMM d, yyyy"));

    // Set greeting based on the time of day
    QString greeting;
    int hour = now.time().hour();  // Get the current hour
    if (hour < 12) greeting = "Good Morning!";  // Morning greeting
    else if (hour < 18) greeting = "Good Afternoon!";  // Afternoon greeting
    else greeting = "Good Evening!";  // Evening greeting
    greetingLabel->setText(greeting);  // Set the greeting label's text

    // Animated clock effect (fade in and out)
    QPropertyAnimation *animation = new QPropertyAnimation(timeLabel, "opacity");  // Create opacity animation
    animation->setDuration(500);  // Set animation duration to 500 milliseconds
    animation->setStartValue(0.5);  // Set initial opacity to 0.5
    animation->setEndValue(1.0);  // Set final opacity to 1.0
    animation->setEasingCurve(QEasingCurve::InOutQuad);  // Use a smooth easing curve
    animation->start(QAbstractAnimation::DeleteWhenStopped);  // Start the animation and delete when finished
}
