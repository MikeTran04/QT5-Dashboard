#include "outdoorTemp.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

/**
 * @class OutdoorTemp
 * @brief Fetches and processes outdoor temperature and weather data from the OpenWeatherMap API.
 * @details outdoorTemp class handles fetching current weather data for London, ON, Canada, from the OpenWeatherMap API.
 *          It processes the received data and emits a signal with the relevant weather information such as
 *          city name, temperature, humidity, and weather condition.
 * 
 * @author Maotong Wei
 */

/**
 * @brief Constructs an OutdoorTemp object.
 * @details This constructor initializes the OutdoorTemp class by setting up a network manager
 *          and connecting the finished signal to the onNetworkReply slot to handle API responses.
 * @param parent The parent QObject (optional, default is nullptr).
 */
OutdoorTemp::OutdoorTemp(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &OutdoorTemp::onNetworkReply);
}

/**
 * @brief Fetches the current temperature and weather data from the OpenWeatherMap API.
 * @details This method sends a request to the OpenWeatherMap API to fetch the current weather data
 *          for London, ON, Canada. The API response is processed asynchronously in the onNetworkReply slot.
 */
void OutdoorTemp::fetchTemperature() {
    // REQUEST OPENWEATHERMAP API FOR CURRENT WEATHER IN LONDON, ON
    QUrl url("http://api.openweathermap.org/data/2.5/weather?q=London,ON,CA&appid=9d8000bf29e9401da98f73684c29c9ef&units=metric");
    QNetworkRequest request(url);
    manager->get(request);
}

/**
 * @brief Handles the network reply containing the weather data.
 * @details This method processes the JSON data returned from the OpenWeatherMap API. It extracts
 *          the city name, temperature, humidity, and weather conditions, and emits the weatherDataFetched signal
 *          with this information. If the API returns an error or the JSON is invalid, it logs a warning.
 * @param reply The network reply containing the weather data.
 */
void OutdoorTemp::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "NETWORK ERROR:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();

        // FETCH CITY NAME
        QString cityName;
        if (obj.contains("name"))
            cityName = obj.value("name").toString();

        // FETCH TEMPERATURE AND HUMIDITY
        double temperature = 0.0;
        double humidity = 0.0;
        if (obj.contains("main") && obj["main"].isObject()) {
            QJsonObject mainObj = obj["main"].toObject();
            if (mainObj.contains("temp"))
                temperature = mainObj["temp"].toDouble();
            if (mainObj.contains("humidity"))
                humidity = mainObj["humidity"].toDouble();
        }

        // FETCH WEATHER CONDITION
        QString condition;
        if (obj.contains("weather") && obj["weather"].isArray()) {
            QJsonArray weatherArr = obj["weather"].toArray();
            if (!weatherArr.isEmpty()) {
                QJsonObject firstWeather = weatherArr.first().toObject();
                if (firstWeather.contains("description"))
                    condition = firstWeather["description"].toString();
            }
        }

        // Emit the weather data
        emit weatherDataFetched(cityName, temperature, humidity, condition);
    } else {
        qWarning() << "The received JSON data is invalid.";
    }
    reply->deleteLater();
}
