#include "motionSensorPage.h"

/**
 * @class MotionSensorPage
 * @brief A class that represents the Motion Sensor page of the Raspberry Pi dashboard.
 * @details This widget displays the current status of the motion sensor, showing whether motion is detected 
 *          or not. It consists of a label that is dynamically updated based on the sensor's input.
 *          The widget's layout is a simple vertical layout containing a centered label.
 * @author Amaan Hafeez, Tianyi Han
 */

/**
 * @brief Constructs a MotionSensorPage object.
 * @details This constructor initializes the MotionSensorPage widget, sets up the layout, and creates a label
 *          to display the motion status. The motion label is initially set to indicate that no motion is detected.
 * @param parent The parent widget for this page (optional, default is nullptr).
 * @author Amaan Hafeez, Tianyi Han
 */
MotionSensorPage::MotionSensorPage(QWidget *parent) : QWidget(parent) {
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);  // Create a vertical layout for the page
    
    // Initialize motion label
    motionLabel = new QLabel("Motion: No motion", this);  // Initialize the label with the initial status
    motionLabel->setAlignment(Qt::AlignCenter);  // Center the label text
    motionLabel->setStyleSheet("font-size: 40px;");  // Set the font size of the label
    
    layout->addWidget(motionLabel);  // Add the motion label to the layout
    setLayout(layout);  // Set the layout for this widget
}

/**
 * @brief Destructor for MotionSensorPage.
 * @details The destructor cleans up any resources used by this page. Currently, no additional cleanup is required.
 */
MotionSensorPage::~MotionSensorPage() {
    // Clean up resources if needed
}

/**
 * @brief Updates the motion status displayed on the page.
 * @details This function updates the text of the motion label based on whether motion is detected or not.
 * @param detected A boolean indicating whether motion was detected (true) or not (false).
 * @return void
 */
void MotionSensorPage::updateMotionStatus(bool detected) {
    motionLabel->setText(detected ? "Motion: Detected!" : "Motion: No motion");
}
