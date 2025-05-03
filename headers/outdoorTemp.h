#ifndef OUTDOORTEMP_H
#define OUTDOORTEMP_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class OutdoorTemp : public QObject {
    Q_OBJECT
public:
    explicit OutdoorTemp(QObject *parent = nullptr);

    // REQUEST OPENWEATHERMAP API FOR CURRENT WEATHER
    void fetchTemperature();

signals:
    // PASS THE WEATHER DATA TO THE UI
    void weatherDataFetched(const QString &cityName,
                            double temperature,
                            double humidity,
                            const QString &condition);

private slots:
    // HANDLE NETWORK REPLIES
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // OUTDOORTEMP_H

