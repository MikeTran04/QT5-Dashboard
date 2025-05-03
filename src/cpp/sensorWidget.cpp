#include "sensorWidget.h"
#include <QDebug>
#include <cmath>

/**
 * @class SensorWidget
 * @brief A widget that displays temperature and humidity sensor data.
 * @details SensorWidget class provides a GUI element that shows temperature and humidity data from a sensor.
 *          It consists of two labels: one for displaying the temperature and one for displaying the humidity. 
 *          If valid data is provided, the labels will be updated accordingly. If the data is invalid, the labels will display "N/A".
 * 
 * @author Amaan Hafeez, Tianyi Han
 */

/**
 * @brief Constructs a SensorWidget object.
 * @details This constructor initializes the widget, sets up a vertical layout, and creates two labels for displaying
 *          temperature and humidity. The layout is configured with specific margins and spacing to ensure the labels
 *          are displayed correctly.
 * @param parent The parent widget for this sensor widget (optional, default is nullptr).
 */
SensorWidget::SensorWidget(QWidget *parent) : QWidget(parent) {
    // Create layout for the widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);  // Set space between widgets
    layout->setContentsMargins(20, 20, 20, 20);  // Set margins around the layout
    
    // Initialize labels for temperature and humidity
    tempLabel = new QLabel("Temperature: N/A", this);  // Label to display temperature
    tempLabel->setAlignment(Qt::AlignCenter);  // Center-align the text in the label
    tempLabel->setStyleSheet("font-size: 40px;");  // Set large font size for readability
    
    humidityLabel = new QLabel("Humidity: N/A", this);  // Label to display humidity
    humidityLabel->setAlignment(Qt::AlignCenter);  // Center-align the text in the label
    humidityLabel->setStyleSheet("font-size: 40px;");  // Set large font size for readability

    // Add labels to the layout
    for (QLabel* label : {tempLabel, humidityLabel}) {
        label->setAlignment(Qt::AlignCenter);  // Center-align label text
        label->setStyleSheet("font-size: 40px;");  // Set the font size of each label
        layout->addWidget(label);  // Add each label to the layout
    }
    
    setLayout(layout);  // Set the layout for the widget
}

/**
 * @brief Destructor for SensorWidget.
 * @details Cleans up any resources used by the widget. Currently, no special cleanup is required as Qt automatically handles memory management for widgets.
 */
SensorWidget::~SensorWidget() {
    // No additional cleanup is needed at this time.
}

/**
 * @brief Updates the displayed sensor data.
 * @details This method takes the temperature and humidity values as parameters, and updates the corresponding labels.
 *          If the provided data is valid (not NaN), it updates the labels with the sensor values. If the data is invalid,
 *          the labels are updated to show "N/A", and a debug message is logged.
 * @param temperature The temperature value to display.
 * @param humidity The humidity value to display.
 * @return void
 */
void SensorWidget::updateSensorData(float temperature, float humidity) {
    // Check if the temperature and humidity values are valid (not NaN)
    if (!std::isnan(temperature) && !std::isnan(humidity)) {
        // Update the labels with the new data
        tempLabel->setText(QString("Temperature: %1 °C").arg(temperature));
        humidityLabel->setText(QString("Humidity: %1 %").arg(humidity));
    } else {
        // Log an error if the sensor data is invalid
        qDebug() << "Failed to read sensor data";
        tempLabel->setText("Temperature: N/A");  // Set label to "N/A" if data is invalid
        humidityLabel->setText("Humidity: N/A");  // Set label to "N/A" if data is invalid
    }
}
