// statswidget.cpp
#include "statswidget.h"
#include <QVBoxLayout>

/**
 * @class StatsWidget
 * @brief A widget that displays real-time statistics for a Raspberry Pi.
 * @details StatsWidget class displays various system statistics such as RAM usage, SD card usage,
 *          CPU temperature, and fan speed. It periodically updates these statistics every 2 seconds using a timer.
 *          The widget layout consists of multiple labels, which are updated with the most recent data from the Raspberry Pi.
 * 
 * @author Amaan Hafeez, Nael Pervez Lakhani
 */

/**
 * @brief Constructs a StatsWidget object.
 * @details The constructor initializes the widget by setting up the labels and layout. It also sets up a timer that
 *          calls the `updateStats` method every 2 seconds to refresh the statistics.
 * @param parent The parent widget for this stats widget (optional, default is nullptr).
 */
StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent), piStats(nullptr) {
    piStats = new PiStats();  // Instantiate PiStats

    createLabels();  // Initialize the labels
    setupLayout();   // Set up the layout for the widget
    
    // Set up timer for periodic updates
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &StatsWidget::updateStats);  // Connect timer to updateStats method
    updateTimer->start(2000);  // Start the timer with an interval of 2000 milliseconds (2 seconds)
}

/**
 * @brief Creates the labels for displaying statistics.
 * @details This method initializes and sets up the labels for displaying the Raspberry Pi statistics, including RAM usage,
 *          SD card usage, CPU temperature, and fan speed. Each label is configured with alignment, style, and default text.
 * @return void
 */
void StatsWidget::createLabels() {
    titleLabel = new QLabel("Raspberry Pi Statistics", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 50px; font-weight: bold; color: #3498db;");

    ramLabel = new QLabel("RAM Usage: Calculating...", this);
    ramLabel->setAlignment(Qt::AlignCenter);
    ramLabel->setStyleSheet("font-size: 40px; color: #2c3e50;");

    sdCardLabel = new QLabel("SD Card Usage: Calculating...", this);
    sdCardLabel->setAlignment(Qt::AlignCenter);
    sdCardLabel->setStyleSheet("font-size: 40px; color: #2c3e50;");

    cpuTempLabel = new QLabel("CPU Temperature: Calculating...", this);
    cpuTempLabel->setAlignment(Qt::AlignCenter);
    cpuTempLabel->setStyleSheet("font-size: 40px; color: #e74c3c;");

    fanSpeedLabel = new QLabel("Fan Speed: Calculating...", this);
    fanSpeedLabel->setAlignment(Qt::AlignCenter);
    fanSpeedLabel->setStyleSheet("font-size: 40px; color: #16a085;");
}

/**
 * @brief Sets up the layout for the widget.
 * @details This method creates a frame to hold the labels and configures the layout for the `StatsWidget` class.
 *          It arranges the labels in a vertical box layout and applies a styled frame around the statistics display.
 * @return void
 */
void StatsWidget::setupLayout() {
    statsFrame = new QFrame(this);
    statsFrame->setFrameShape(QFrame::StyledPanel);
    statsFrame->setFrameShadow(QFrame::Raised);
    statsFrame->setLineWidth(3);
    statsFrame->setStyleSheet("QFrame { background-color: rgba(255, 255, 255, 0.8); border-radius: 15px; }");

    QVBoxLayout* frameLayout = new QVBoxLayout(statsFrame);
    frameLayout->setContentsMargins(100, 100, 100, 100);  // Set margins for the frame layout
    frameLayout->setSpacing(15);  // Set space between widgets in the frame layout
    
    frameLayout->addWidget(titleLabel);   // Add title label to the layout
    frameLayout->addWidget(ramLabel);     // Add RAM usage label to the layout
    frameLayout->addWidget(sdCardLabel);  // Add SD card usage label to the layout
    frameLayout->addWidget(cpuTempLabel); // Add CPU temperature label to the layout
    frameLayout->addWidget(fanSpeedLabel);// Add fan speed label to the layout
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(statsFrame);  // Add the stats frame to the main layout
    setLayout(mainLayout);  // Set the layout for the widget
}

/**
 * @brief Updates the displayed statistics.
 * @details This method retrieves the latest statistics from the `piStats` object and updates the corresponding labels.
 *          It sets the text of each label based on the values returned from the `piStats` object. The statistics updated
 *          include RAM usage, SD card usage, CPU temperature, and fan speed.
 * @return void
 */
void StatsWidget::updateStats() {
    if (!piStats) {
        return;
    }

    
    // Retrieve the latest statistics from the piStats object
    double ramUsage = piStats->getRAMUsage();
    double sdUsage = piStats->getSDCardUsage();
    double cpuTemp = piStats->getCPUTemperature();
    int fanSpeed = piStats->getFanSpeed();

    // Log the statistics to the debug output

    // Update the labels with the retrieved statistics
    ramLabel->setText(QString("RAM Usage: %1%").arg(ramUsage, 0, 'f', 1));
    sdCardLabel->setText(QString("SD Card Usage: %1%").arg(sdUsage, 0, 'f', 1));
    cpuTempLabel->setText(QString("CPU Temperature: %1°C").arg(cpuTemp, 0, 'f', 1));
    fanSpeedLabel->setText(QString("Fan Speed: %1 RPM").arg(fanSpeed));
}

/**
 * @brief Destructor for StatsWidget.
 * @details The destructor stops the update timer if it is active and cleans up any resources used by the widget.
 * @return void
 */
StatsWidget::~StatsWidget() {
    // If you have any resources to clean up, do it here
    // For example, stop timers if needed:
    if (updateTimer->isActive()) {
        updateTimer->stop();  // Stop the update timer when the widget is destroyed
    }
}
