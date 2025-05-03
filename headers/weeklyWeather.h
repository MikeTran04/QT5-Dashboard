#ifndef WEEKLYWEATHER_H
#define WEEKLYWEATHER_H

#include <QObject>
#include <QJsonObject>

class QNetworkAccessManager;
class QNetworkReply;

class WeeklyWeather : public QObject {
    Q_OBJECT
public:
    explicit WeeklyWeather(QObject *parent = nullptr);

    void fetchWeeklyWeather();

signals:
    void weeklyWeatherFetched(const QJsonObject &data);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // WEEKLYWEATHER_H
