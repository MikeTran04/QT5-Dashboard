#include "dashboard.h"
#include "statswidget.h"
#include <QDateTime>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QTouchEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <sys/statvfs.h>

/**
 * @class Dashboard
 * @brief Represents the main user interface for the smart dashboard.
 * @details The Dashboard class manages various UI components such as the mode switching button, stacked widgets for different pages, brightness control, and sensor data. It supports running on a Raspberry Pi and includes functionality like touch events, proximity sensing, and motion detection.
 * 
 * @author Mike Tran, Amaan Hafeez, Nael Pervez Lakhani, Tianyi Han
 */


/**
 * @brief Constructor for the Dashboard class.
 * @param onRaspberryPi A boolean indicating if the dashboard is running on a Raspberry Pi.
 * @param parent The parent QWidget for the Dashboard.
 */
// Constructor for Dashboard class: takes boolean param onRaspberryPi to determine if dashboard is running on actual hardware.
// Using currentIndex allows for only one page to be visible at a time on the dashboard - allowing for use of stacked widgets
Dashboard::Dashboard(bool onRaspberryPi, QWidget *parent) 
    : QWidget(parent), 
      currentIndex(0),
      // create brightness control and active and off mode instances for managing screen brightness
      brightnessController(new BrightnessControl(this)),   
      activeMode(new ActiveMode(brightnessController, this)),  
      offMode(new OffMode(brightnessController, this)),
      piStats(nullptr),  // Initialize PiStats pointer to null
      m_onRaspberryPi(onRaspberryPi)
{
    setFocusPolicy(Qt::StrongFocus);  // ensures the widget to recieve keyboard focus
    setAttribute(Qt::WA_AcceptTouchEvents); // Enable touch events
    
    // initialize components
    // initializeLightSensor(); // light sensor to enable automatic brightness adjustment
    initializeUI(); // set up UI elements and stacked widget structure
    initializeConnections(); // connect signals and slots for communication between different components of the dashboard.
    
    // initialize hardware sensors only when running on Raspberry Pi: m_onRaspberryPi is a class member variable storing boolean. actual detection of raspberry pi happens in main using the isRaspberryPi() function.
    if (m_onRaspberryPi) {
        initializeSensors();
        piStats = new PiStats();
    }
    
    //brightnessTest(); // set initial brightness level for dashboard
}


/**
 * @brief Initializes the ambient light sensor for automatic brightness adjustment.
 */
void Dashboard::initializeLightSensor() {
    // create light sensor worker to run in a separate thread to continiously adjust screen brightness based on ambient light. 
    sensorThread = new QThread(this);
    lightSensorWorker = new LightSensorWorker();
    lightSensorWorker->moveToThread(sensorThread);

    // move to thread and set up connections and start the light sensor thread.
    connect(sensorThread, &QThread::started, lightSensorWorker, &LightSensorWorker::readLightSensor);
    connect(lightSensorWorker, &LightSensorWorker::lightLevelUpdated, brightnessController, &BrightnessControl::updateBrightness);

    // Start the thread
    sensorThread->start();
}



/**
 * @brief Creates and sets up the main UI layout and stacked widget for different UI pages.
 */
void Dashboard::initializeUI() {
    // create main UI layout
    layout = new QVBoxLayout(this);
    
    // create and setup stacked widget - allowing for multiple pages on a single GUI window that appears once at a time based on currentIndex.
    stackedWidget = new QStackedWidget(this);
    
    // mode button creation: creates mode button allowing users to switch modes between active and off. 
    modeButton = new QPushButton("Switch to OffMode", this);
    modeButton->setStyleSheet("font-size: 20px; font-weight: bold;");
    modeButton->adjustSize();

    // Sleep notification when in sleep mode
    sleepLabel = new QLabel(this);
    
    // Configure all widgets for the stacked widget
    setupDateTimeWidgets();
    setupMotionWidget();
    setupSensorWidget();
    setupWeatherWidget();
    setupYouTubeWidget();
    setupYouTubeMusicWidget();
    setupGoogleCalendarWidget(); // Add Google Calendar widget setup
    setupStatsWidget();
    
    // Configure main layout
    layout->addWidget(stackedWidget);

    // Add dimOverlay to the layout with the appropriate stretch factor
    layout->addWidget(modeButton, 0, Qt::AlignTop | Qt::AlignHCenter);
    setLayout(layout);

    // Add overlay for dimming effect
    dimOverlay = new QWidget(this);
    dimLayout = new QVBoxLayout(dimOverlay);
}


/**
 * @brief Initializes the widgets for displaying the current date and time and add it to the stacked widget.
 */
void Dashboard::setupDateTimeWidgets() {
    // Create DateTimePage instance and add it to stacked widget
    dateTimePage = new DateTimePage(this);
    stackedWidget->addWidget(dateTimePage);
}

/**
 * @brief Creates and adds the motion detection page to the stacked widget.
 */
void Dashboard::setupMotionWidget() {
    motionSensorPage = new MotionSensorPage(this);
    //stackedWidget->addWidget(motionSensorPage);
}

/**
 * @brief Creates and adds the sensor page to display temperature and humidity data and adds to stacked widget.
 */
void Dashboard::setupSensorWidget() {
    sensorWidget = new SensorWidget(this);
    //stackedWidget->addWidget(sensorWidget);
}

/**
 * @brief Creates and adds the weather station page to display real-time weather data to the stacked widget.
 */
void Dashboard::setupWeatherWidget() {
    weatherStationPage = new WeatherStation(this);
    stackedWidget->addWidget(weatherStationPage);
}

/**
 * @brief Creates and adds the YouTube page to the stacked widget and starts the Flask server.
 */
void Dashboard::setupYouTubeWidget() {
    youtubePage = new YouTubePage(this);
    stackedWidget->addWidget(youtubePage);
    
    // Start Flask server for YouTube authentication
    flaskProcess = new QProcess(this);
    flaskProcess->start("python3", QStringList() << "youtube_server.py");
}

/**
 * @brief Creates and adds the YouTube Music page to the stacked widget.
 */
void Dashboard::setupYouTubeMusicWidget() {
    youtubeMusicPage = new YouTubeMusicPage(this);
    stackedWidget->addWidget(youtubeMusicPage);
}

/**
 * @brief Creates and adds the raspberry pi stats page to the stacked widget.
 */
void Dashboard::setupStatsWidget() {
    StatsWidget* statsWidget = new StatsWidget(this);
    
    // If currently running on Raspberry Pi, set the stats object
    if (m_onRaspberryPi && piStats) {
        statsWidget->setPiStats(piStats);
    }
    
    stackedWidget->addWidget(statsWidget);
}

/**
 * @brief Creates and adds the Google Calendar page to the stacked widget.
 */
void Dashboard::setupGoogleCalendarWidget() {
    googleCalendarPage = new GoogleCalendarPage(this);
    stackedWidget->addWidget(googleCalendarPage);
}

/**
 * @brief Sets up connections for signals and slots.
 */
void Dashboard::initializeConnections() {
    // Connect mode and brightness signals/slots
    connect(brightnessController, &BrightnessControl::brightnessChanged, this, &Dashboard::backgroundUpdate);
    connect(modeButton, &QPushButton::clicked, this, &Dashboard::toggleMode);
    connect(activeMode, &ActiveMode::modeChanged, this, &Dashboard::handleModeChanged);
    connect(offMode, &OffMode::modeChanged, this, &Dashboard::handleModeChanged);

    // ---------------------------------------------------------------------
    // Final Initialization / Testing
    // ---------------------------------------------------------------------
    //brightnessTest();
    //proximityOffModeTest();
    //proximityActiveModeTest();
}

/**
 * @brief Initializes motion and environmental sensors for the dashboard.
 */
void Dashboard::initializeSensors() {
    // Set up motion sensor
    motionSensor = new HCSR501(7, m_onRaspberryPi, this);
    motionTimer = new QTimer(this);
    connect(motionTimer, &QTimer::timeout, motionSensor, &HCSR501::checkMotion);
    connect(motionSensor, &HCSR501::motionDetected, motionSensorPage, &MotionSensorPage::updateMotionStatus);
    motionTimer->start(500);
    
    // Set up temperature/humidity sensor
    sensorWorker = new SensorWorker(0, m_onRaspberryPi);
    workerThread = new QThread(this);
    sensorWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, sensorWorker, &SensorWorker::doWork);
    connect(sensorWorker, &SensorWorker::sensorDataReady, this, &Dashboard::handleSensorData);
    workerThread->start();
    
    sensorTimer = new QTimer(this);
    connect(sensorTimer, &QTimer::timeout, sensorWorker, &SensorWorker::doWork);
    sensorTimer->start(500);
}

/**
 * @brief Destructor for cleaning up resources, including threads, workers, and other components.
 */
Dashboard::~Dashboard() {
    // Stop and clean up sensor threads
    if (sensorThread && sensorThread->isRunning()) {
        sensorThread->quit();
        sensorThread->wait(3000); // Wait up to 3 seconds for thread to finish
        if (!sensorThread->isFinished()) {
            sensorThread->terminate(); // Force termination if necessary
        }
        delete sensorThread;
    }
    
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait(3000); // Wait up to 3 seconds for thread to finish
        if (!workerThread->isFinished()) {
            workerThread->terminate(); // Force termination if necessary
        }
        delete workerThread;
    }
    
    // Delete worker objects
    if (lightSensorWorker) {
        delete lightSensorWorker;
    }
    
    if (sensorWorker) {
        delete sensorWorker;
    }
    
    // Clean up timers
    if (sensorTimer) {
        sensorTimer->stop();
        delete sensorTimer;
    }
    
    if (motionTimer) {
        motionTimer->stop();
        delete motionTimer;
    }

    // Clean up existing resources
    delete modeButton;
    delete activeMode;
    delete offMode;
    delete brightnessController;

    // Add Flask process cleanup
    if (flaskProcess) {
        flaskProcess->terminate();
        flaskProcess->waitForFinished(3000); // Wait up to 3 seconds
        if (flaskProcess->state() != QProcess::NotRunning) {
            flaskProcess->kill(); // Force kill if still running
        }
        delete flaskProcess;
    }
    
    if (piStats) {
        delete piStats;
    }
    
    // Clean up motion sensor
    if (motionSensor) {
        delete motionSensor;
    }
}

/**
 * @brief Updates the sensor widget with the latest temperature and humidity data.
 * @param temperature The latest temperature reading.
 * @param humidity The latest humidity reading.
 */
void Dashboard::handleSensorData(float temperature, float humidity) {
    sensorWidget->updateSensorData(temperature, humidity);
}

/**
 * @brief Updates the proximity sensor data.
 * @param range The proximity range measurement.
 */
void Dashboard::proximitySensorUpdate(double range) {
    activeMode->monitorProximity(range);
    offMode->monitorProximity(range);
}

/**
 * @brief Toggles between ActiveMode and OffMode based on the current state.
 */
void Dashboard::toggleMode() {
    qDebug() << "Toggle Mode";
    if (modeButton->text() == "Switch to OffMode") {
        qDebug() << "Go to OffMode";
        proximityActive = true; // this will trigger the proximity data collector to send data to proximitySensorUpdate()
        activeMode->setSleepStatus(false);
        activeMode->exitActiveMode();
    } else {
        qDebug() << "Go to ActiveMode";
        proximityActive = false; // this will stop trigger the proximity data collector to send data to proximitySensorUpdate()
        offMode->exitOffMode();
    }
}

/**
 * @brief Handles the mode changes and updates the mode button accordingly.
 * @param isActive A boolean indicating if the active mode is active.
 */
void Dashboard::handleModeChanged(bool isActive) {
    if (isActive) {
        modeButton->setText("Switch to OffMode");
        if (!activeMode->getIsActive()) activeMode->enterActiveMode();
    } else {
        modeButton->setText("Switch to ActiveMode");
        if (!offMode->getIsOff()) offMode->enterOffMode();
    }
}


/**
 * @brief Updates the background dimming overlay based on the current brightness level.
 */
void Dashboard::backgroundUpdate() {
    //qDebug() << "updating brightness";  // DEBUG
    float opacity = 1.0 - (brightnessController->getCurrentBrightness() / 100.0);
    
    //qDebug() << "opacity: " << opacity;  // DEBUG
    int alpha = static_cast<int>(opacity * 255);

    // Update the overlay with the dimming effect
    dimOverlay->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    dimOverlay->setStyleSheet(QString("background: rgba(0, 0, 0, %1);").arg(alpha));
    dimOverlay->setGeometry(0, 0, 1920, 1080);  // Adjust to full size

    modeButton->raise();

    
    qDebug() << "sleepStatus: " << activeMode->getSleepStatus();
    qDebug() << "offMode: " << offMode->getIsOff();
    if (activeMode->getSleepStatus() && offMode->getIsOff()) {
        // Center the sleep label
        sleepLabel->setText("In Sleeping Mode...");
        sleepLabel->setAlignment(Qt::AlignHCenter);
        sleepLabel->setStyleSheet("font-size: 40px; color: rgba(20, 23, 105, 0.84); font-weight: bold; font-family: 'Orbitron', sans-serif;");
        sleepLabel->setVisible(true);

        dimLayout->addWidget(sleepLabel, 0, Qt::AlignCenter);
        dimOverlay->setLayout(dimLayout);
        sleepLabel->raise();
    } else sleepLabel->setVisible(false);
    
}


/**
 * @brief Paint event to draw the background image and the brightness overlay.
 * @param event The paint event.
 */
void Dashboard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bgPixmap(":/images/1.jpg");
    bgPixmap = bgPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, bgPixmap);
    QWidget::paintEvent(event);
}

/**
 * @brief Handles key press events for navigation and quitting the application.
 * @param event The key press event.
 */
void Dashboard::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_C && event->modifiers() & Qt::ControlModifier)
        QApplication::quit();
    else {
        switch (event->key()) {
            case Qt::Key_Left:
                currentIndex = (currentIndex - 1 + stackedWidget->count()) % stackedWidget->count();
                stackedWidget->setCurrentIndex(currentIndex);
                qDebug() << "current index:" << currentIndex;
                break;
            case Qt::Key_Right:
                currentIndex = (currentIndex + 1) % stackedWidget->count();
                stackedWidget->setCurrentIndex(currentIndex);
                qDebug() << "current index:" << currentIndex;
                break;
            default:
                QWidget::keyPressEvent(event);
                break;
        }
    }
}

/**
 * @brief Handles touch events to navigate between stacked widgets.
 * @param event The touch event.
 * @return A boolean indicating if the event was handled.
 */
bool Dashboard::event(QEvent *event) {
    if (event->type() == QEvent::TouchBegin || 
        event->type() == QEvent::TouchUpdate || 
        event->type() == QEvent::TouchEnd)
    {
        touchEvent(static_cast<QTouchEvent *>(event));
        return true;
    }
    return QWidget::event(event);
}

/**
 * @brief Processes touch events for navigating between stacked widgets.
 * @param event The touch event.
 */
void Dashboard::touchEvent(QTouchEvent *event) {
    if (event->touchPoints().size() == 1) {
        QTouchEvent::TouchPoint touchPoint = event->touchPoints().first();
        static QPoint lastPos;
        if (event->type() == QEvent::TouchBegin)
            lastPos = touchPoint.pos().toPoint();
        else if (event->type() == QEvent::TouchEnd) {
            int deltaX = touchPoint.pos().x() - lastPos.x();
            if (deltaX > 50)
                currentIndex = (currentIndex - 1 + stackedWidget->count()) % stackedWidget->count();
            else if (deltaX < -50)
                currentIndex = (currentIndex + 1) % stackedWidget->count();

            qDebug() << "current index (touch): " << currentIndex;
            
            stackedWidget->setCurrentIndex(currentIndex);
        }
    } else if (event->touchPoints().size() == 2) {
        // Handle two-finger swipe
        QTouchEvent::TouchPoint firstPoint = event->touchPoints().at(0);
        QTouchEvent::TouchPoint secondPoint = event->touchPoints().at(1);
        static QPoint lastFirstPos, lastSecondPos;

        if (event->type() == QEvent::TouchBegin) {
            lastFirstPos = firstPoint.pos().toPoint();
            lastSecondPos = secondPoint.pos().toPoint();
        } else if (event->type() == QEvent::TouchEnd) {
            int deltaXFirst = firstPoint.pos().x() - lastFirstPos.x();
            int deltaXSecond = secondPoint.pos().x() - lastSecondPos.x();

            // Check if both fingers swiped left
            if (deltaXFirst < -50 && deltaXSecond < -50) {
                qDebug() << "Two-finger left swipe detected. Navigating to YouTube Music.";
                // Navigate directly to YouTube Music page
                for (int i = 0; i < stackedWidget->count(); ++i) {
                    if (stackedWidget->widget(i) == youtubeMusicPage) {
                        currentIndex = i;
                        stackedWidget->setCurrentIndex(currentIndex);
                        break;
                    }
                }
            }
        }
    } else if (event->touchPoints().size() == 3) {
        // Handle three-finger swipe
        QTouchEvent::TouchPoint firstPoint = event->touchPoints().at(0);
        QTouchEvent::TouchPoint secondPoint = event->touchPoints().at(1);
        QTouchEvent::TouchPoint thirdPoint = event->touchPoints().at(2);
        static QPoint lastFirstPos, lastSecondPos, lastThirdPos;

        if (event->type() == QEvent::TouchBegin) {
            lastFirstPos = firstPoint.pos().toPoint();
            lastSecondPos = secondPoint.pos().toPoint();
            lastThirdPos = thirdPoint.pos().toPoint();
        } else if (event->type() == QEvent::TouchEnd) {
            int deltaXFirst = firstPoint.pos().x() - lastFirstPos.x();
            int deltaXSecond = secondPoint.pos().x() - lastSecondPos.x();
            int deltaXThird = thirdPoint.pos().x() - lastThirdPos.x();

            // Check if all three fingers swiped left
            if (deltaXFirst < -50 && deltaXSecond < -50 && deltaXThird < -50) {
                qDebug() << "Three-finger left swipe detected. Navigating to Google Calendar.";
                // Navigate directly to Google Calendar page
                for (int i = 0; i < stackedWidget->count(); ++i) {
                    if (stackedWidget->widget(i) == googleCalendarPage) {
                        currentIndex = i;
                        stackedWidget->setCurrentIndex(currentIndex);
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief Tests the brightness adjustment functionality by simulating an ambient light level.
 * This function adjusts the screen brightness based on the simulated ambient light level.
 */
void Dashboard::brightnessTest() {
    int ambientLightLevel = 1000;
    brightnessController->adjustBrightness(ambientLightLevel);
}

/**
 * @brief Tests the proximity detection and mode transition for Active Mode.
 * This function simulates the monitoring of proximity and enters Active Mode if the range is below a threshold.
 */
void Dashboard::proximityActiveModeTest() {
    qDebug() << "is this working";
    activeMode->monitorProximity(11); 
}

/**
 * @brief Tests the proximity detection and mode transition for Off Mode.
 * This function simulates entering Off Mode and then checks proximity after a delay.
 */
void Dashboard::proximityOffModeTest() {
    offMode->enterOffMode();
    QTimer::singleShot(10000, this, [this]() {
        offMode->monitorProximity(8);
    });
}
