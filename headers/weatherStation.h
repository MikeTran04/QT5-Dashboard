#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QStackedWidget>   // For internal page switching between the main page, trend graph, and alert pages
#include <QProgressBar>

class OutdoorTemp;    // Forward declaration for OutdoorTemp class (handles fetching outdoor temperature data)
class trendGraphPage; // Forward declaration for trendGraphPage class (handles the trend graph page)
class AlertPage;      // Forward declaration for AlertPage class (handles the alert page)

class WeatherStation : public QWidget {
    Q_OBJECT

public:
    explicit WeatherStation(QWidget *parent = nullptr); // Constructor to initialize the WeatherStation widget

signals:
    // Signal declarations
    void sensorMonitorRequested(); // Emitted when the sensor monitoring button is clicked
    void trendGraphRequested();    // Emitted when the trend graph button is clicked
    void alertPageRequested();     // Emitted when the alert page button is clicked

public slots:
    void updateWeatherInfo(); // Slot to update weather information from the outdoor sensor
    void updateIndoorSensorData(float temperature, float humidity);  // New slot

protected:
    void keyPressEvent(QKeyEvent *event) override; // Handle key press events (e.g., Esc key)

private slots:
    // Private slots to handle fetched weather data, update indoor info, and switch between pages
    void handleWeatherData(const QString &cityName,
                           double temperature,
                           double humidity,
                           const QString &condition);
    void updateIndoorInfo();    // Slot to simulate indoor temperature updates
    void updateDisplay();       // Slot to update the UI with the latest data

    // Subpage switching functions for the internal stacked widget (for switching between pages)
    void showTrendGraphPage();
    void onTrendGraphBack();     // Go back to the main content from the trend graph page
    void showAlertPage();        // Show the alert page
    void onAlertPageBack();      // Go back to the main content from the alert page

private:
    // Main content elements (Widgets for displaying weather information)
    QWidget *mainContentWidget = nullptr;  // Store the main content widget (contains all UI elements)
    QLabel *weatherCityLabel = nullptr;    // Label to display the city name
    QLabel *weatherTempLabel = nullptr;    // Label to display the outdoor temperature
    QLabel *weatherHumidityLabel = nullptr; // Label to display the humidity percentage
    QLabel *weatherConditionLabel = nullptr; // Label to display the current weather condition (e.g., sunny, rainy)
    QLabel *indoorDataLabel = nullptr;     // Label to display indoor temperature

    // Buttons for navigating between pages
    QPushButton *sensorButton = nullptr;   // Button for sensor monitoring
    QPushButton *trendButton = nullptr;    // Button for viewing the trend graph
    QPushButton *alertButton = nullptr;    // Button for viewing real-time weather advisories

    // Timers for updating outdoor and indoor data
    QTimer *outdoorTimer = nullptr; // Timer to periodically fetch outdoor weather data (every 30 minutes)
    QTimer *indoorTimer = nullptr;  // Timer to periodically update indoor temperature data (every second)

    // Pointer to the OutdoorTemp object, which fetches outdoor temperature and humidity
    OutdoorTemp *outdoorTemp = nullptr;
    QString latestCity;              // Stores the name of the city fetched from the outdoor sensor
    double latestOutdoorTemp = 0.0;  // Stores the latest outdoor temperature
    double latestHumidity = 0.0;     // Stores the latest outdoor humidity
    QString latestCondition;         // Stores the latest weather condition (e.g., sunny, cloudy)
    double indoorTemp = 22.0;        // Stores the current indoor temperature (initialized to 22°C)
    float indoorHumidity = 0.0;      // Stores the current indoor humidity

    // Pointers to the pages (trend graph page and alert page)
    trendGraphPage *trendPage = nullptr; 
    AlertPage *alertPage = nullptr;

    // Stacked widget for switching between subpages (trend graph, alert page, etc.)
    QStackedWidget *internalStackedWidget = nullptr;

    // Progress bars for displaying the temperature and humidity visually
    QProgressBar *temperatureBar = nullptr;
    QProgressBar *humidityBar = nullptr;
};

#endif // WEATHERSTATION_H
