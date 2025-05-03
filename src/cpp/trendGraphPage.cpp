// trendGraphPage.cpp
#include "trendGraphPage.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>

QT_CHARTS_USE_NAMESPACE

/**
 * @class trendGraphPage
 * @brief A page that displays temperature trends in graphical form.
 * @details trendGraphPage class displays two charts: one for the 7-day temperature forecast
 *          and another for the next 24-hour temperature forecast. The data for these charts is updated
 *          from the provided JSON data, which includes daily and hourly temperature forecasts.
 *          The page also includes a back button to navigate to a previous view.
 * 
 * @author Maotong Wei
 */

/**
 * @brief Constructs a trendGraphPage object.
 * @details The constructor initializes the layout, creates a back button, and sets up two chart views (weekly and hourly).
 *          It connects the back button to the `backRequested` signal, which is emitted when the button is clicked.
 * @param parent The parent widget for this page (optional, default is nullptr).
 */
trendGraphPage::trendGraphPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // BACK BUTTON
    QString buttonStyle = "font-size: 16px; padding: 8px; "
                          "font-weight: bold; "   // Blue background
                          "background-color:rgb(63, 180, 100); " 
                          "color: white; "                 // White text
                          "border: 5px solidrgb(84, 0, 158); ";    // Slight border for contrast

    QPushButton *backButton = new QPushButton("Back", this);
    backButton->setStyleSheet(buttonStyle);
    connect(backButton, &QPushButton::clicked, this, [=](){
        emit backRequested();  // Emit backRequested signal when the button is clicked
    });
    layout->addWidget(backButton);

    // CREATE CHART VIEWS
    weeklyChartView = new QtCharts::QChartView(this);
    hourlyChartView = new QtCharts::QChartView(this);
    layout->addWidget(weeklyChartView, 1);  // Add weekly chart view to the layout
    layout->addWidget(hourlyChartView, 1);  // Add hourly chart view to the layout

    setLayout(layout);  // Set the layout for the page
}

/**
 * @brief Updates the weekly chart with new data.
 * @details This method updates the weekly chart by parsing the provided JSON data, which contains the 7-day forecast.
 *          It extracts the temperature and date information, populates a line series, and updates the chart.
 *          The chart is configured with date formatting on the X-axis and temperature on the Y-axis.
 * @param jsonData A JSON object containing the 7-day forecast data.
 * @return void
 */
void trendGraphPage::updateWeeklyChartData(const QJsonObject &jsonData) {
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QJsonArray dailyArray = jsonData["daily"].toArray();
    for (int i = 0; i < dailyArray.size(); ++i) {
        QJsonObject dayObj = dailyArray[i].toObject();
        double temp = dayObj["temp"].toObject()["day"].toDouble();  // Get the daily temperature
        qint64 dt = dayObj["dt"].toVariant().toLongLong();  // Get the timestamp
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(dt);  // Convert the timestamp to QDateTime
        series->append(dateTime.toMSecsSinceEpoch(), temp);  // Add the data point to the series
    }
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Future 7-Day Temperature Forecast");
    chart->legend()->hide();  // Hide the chart legend
    QtCharts::QDateTimeAxis *axisX = new QtCharts::QDateTimeAxis;
    axisX->setFormat("MM-dd");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setTitleText("Temperature (°C)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    weeklyChartView->setChart(chart);  // Set the updated chart to the weekly chart view
}

/**
 * @brief Updates the hourly chart with new data.
 * @details This method updates the hourly chart by parsing the provided JSON data, which contains the hourly forecast.
 *          It extracts the temperature and time information, populates a line series, and updates the chart.
 *          The chart is configured with time formatting on the X-axis and temperature on the Y-axis.
 * @param jsonData A JSON object containing the hourly forecast data.
 * @return void
 */
void trendGraphPage::updateHourlyChartData(const QJsonObject &jsonData) {
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QJsonArray hourlyArray = jsonData["hourly"].toArray();
    int count = qMin(hourlyArray.size(), 24);  // Limit the number of data points to 24 (next 24 hours)
    for (int i = 0; i < count; ++i) {
        QJsonObject hourObj = hourlyArray[i].toObject();
        double temp = hourObj["temp"].toDouble();  // Get the hourly temperature
        qint64 dt = hourObj["dt"].toVariant().toLongLong();  // Get the timestamp
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(dt);  // Convert the timestamp to QDateTime
        series->append(dateTime.toMSecsSinceEpoch(), temp);  // Add the data point to the series
    }
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Next 24-Hour Temperature Forecast");
    chart->legend()->hide();  // Hide the chart legend
    QtCharts::QDateTimeAxis *axisX = new QtCharts::QDateTimeAxis;
    axisX->setFormat("HH:mm");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setTitleText("Temperature (°C)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    hourlyChartView->setChart(chart);  // Set the updated chart to the hourly chart view
}
