#ifndef HOURLYWEATHER_H
#define HOURLYWEATHER_H

#include <QObject>
#include <QJsonObject>

class QNetworkAccessManager;
class QNetworkReply;

class HourlyWeather : public QObject {
    Q_OBJECT
public:
    explicit HourlyWeather(QObject *parent = nullptr);
    void fetchHourlyWeather();
signals:
    void hourlyWeatherFetched(const QJsonObject &data);
private slots:
    void onNetworkReply(QNetworkReply *reply);
private:
    QNetworkAccessManager *manager;
};

#endif // HOURLYWEATHER_H
