#ifndef DASHBOARD_H
#define DASHBOARD_H
#include <QWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QProcess>
#include <QKeyEvent>
#include <QTouchEvent>
#include <QPainter>
#include <QThread>
#include <QPushButton>
#include <QStackedLayout>

// class headers
#include "weatherStation.h" 
#include "brightness_control.h"
#include "active_mode.h"
#include "off_mode.h"
#include "dateTimePage.h"
#include "sensorWidget.h"
#include "YouTubeMusicPage.h"
#include "YouTubePage.h"
#include "motionSensorPage.h"
#include "GoogleCalendarPage.h" // Import Google Calendar page

// sensor headers
#include "piStats.h"
#include "hcsr501.h"
#include "sensorworker.h"
#include "lightsensorworker.h"

class StatsWidget;

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(bool onRaspberryPi, QWidget *parent = nullptr);
    virtual ~Dashboard();  
    WeatherStation* getWeatherStation() const {
        return weatherStationPage;  
    }

    YouTubeMusicPage* getYouTubeMusicPage() const {
        return youtubeMusicPage;  
    }

    YouTubePage* getYouTubePage() const {
        return youtubePage;  
    }

    DateTimePage* getDateTimePage() const {
        return dateTimePage;  
    }

    GoogleCalendarPage* getCalendarPage() const {
        return googleCalendarPage;;  
    }



    protected:
    bool event(QEvent *event) override; // handle general events
    void touchEvent(QTouchEvent *event);  // handle touch events
    void keyPressEvent(QKeyEvent *event) override; // handle key press events
    void paintEvent(QPaintEvent *event) override;  // handle paint events
    void backgroundUpdate(); // handle background UI updates

    void brightnessTest();  // test brightness adjustment
    void proximityActiveModeTest();  // test proximity sensor in ActiveMode
    void proximityOffModeTest();  // test proximity sensor in OffMode

public slots:
    void toggleMode();  // slot to toggle between ActiveMode and OffMode
    void handleModeChanged(bool isActive);  // handle mode change signals (Active/Off Mode)
    void proximitySensorUpdate(double range); // update UI based on proximity sensor data
    void handleSensorData(float temperature, float humidity); // handle sensor data updates
    

private:
    void initializeLightSensor();
    void initializeUI();
    void setupDateTimeWidgets();
    void setupMotionWidget();
    void setupSensorWidget();
    void setupWeatherWidget();
    void setupYouTubeWidget();
    void setupYouTubeMusicWidget();
    void setupStatsWidget();
    void setupGoogleCalendarWidget(); // Declare the function here
    void initializeConnections();
    void initializeSensors();
    
    int currentIndex = 0;   // current index for navigating UI views
    BrightnessControl *brightnessController;
    ActiveMode *activeMode;
    OffMode *offMode;
    PiStats *piStats = nullptr;  // pointer to the statistics helper class
    bool m_onRaspberryPi;  // flag to track if running on Raspberry Pi  
    bool proximityActive = true;  
    
    // UI elements
    QWidget *dimOverlay;
    QVBoxLayout *dimLayout;
    QVBoxLayout *layout;
    QStackedWidget *stackedWidget; // manage different UI views
    DateTimePage *dateTimePage; // date and time component
    SensorWidget *sensorWidget;
    MotionSensorPage *motionSensorPage; // motion detection display
    QTimer *sensorTimer;  // timer for sensor readings
    QTimer *motionTimer;  // timer for motion sensor checks
    QLabel *sleepLabel; 

    // sensor and worker objects
    SensorWorker *sensorWorker;  // worker to read sensor data
    HCSR501 *motionSensor; // Motion sensor object
    LightSensorWorker *lightSensorWorker;  // Light sensor worker instance
    QThread *workerThread;  // Thread for sensor worker
    QThread *sensorThread;  // Thread for light sensor

    QPushButton *modeButton;
    
    // YouTube Integration
    QProcess *flaskProcess;          // Process for running the Flask server
    YouTubePage *youtubePage;        // YouTube component
    YouTubeMusicPage *youtubeMusicPage; // YouTube Music component
    WeatherStation *weatherStationPage;
    GoogleCalendarPage *googleCalendarPage; // Google Calendar component
};

#endif
