#ifndef TRENDGRAPHPAGE_H
#define TRENDGRAPHPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QtCharts/QChartView> 

QT_BEGIN_NAMESPACE
namespace QtCharts {
    class QChart;
    class QLineSeries;
    class QDateTimeAxis;
    class QValueAxis;
}
QT_END_NAMESPACE

class trendGraphPage : public QWidget {
    Q_OBJECT
public:
    explicit trendGraphPage(QWidget *parent = nullptr);

    // UPDATE WEEKLY CHART DATA
    void updateWeeklyChartData(const QJsonObject &jsonData);
    // UPDATE HOURLY CHART DATA
    void updateHourlyChartData(const QJsonObject &jsonData);

signals:
    void backRequested(); 

private:
    // DISPLAY WEEKLY AND HOURLY CHARTS
    QtCharts::QChartView *weeklyChartView;
    QtCharts::QChartView *hourlyChartView;
};

#endif // TRENDGRAPHPAGE_H

