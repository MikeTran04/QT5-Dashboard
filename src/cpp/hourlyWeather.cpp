#include "hourlyWeather.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

/**
 * @class HourlyWeather
 * @brief A class for fetching hourly weather data from the OpenWeatherMap API.
 * @details This class interacts with the OpenWeatherMap API to retrieve hourly weather data
 *          for a specific geographic location. It uses Qt's network module to send a GET request
 *          to the API and processes the response by extracting relevant weather information.
 *          The class emits a signal `hourlyWeatherFetched` when the data is successfully fetched.
 * @author Maotong Wei
 */
HourlyWeather::HourlyWeather(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);  // Initialize QNetworkAccessManager for network requests
    connect(manager, &QNetworkAccessManager::finished,
            this, &HourlyWeather::onNetworkReply);  // Connect finished signal to onNetworkReply slot
}

/**
 * @brief Fetches the hourly weather data from the OpenWeatherMap API.
 * @details This function sends a GET request to the OpenWeatherMap API for hourly weather data,
 *          excluding unnecessary data like daily weather, alerts, and minutely data.
 *          The data is retrieved in metric units (Celsius).
 *          After sending the request, the function waits for the response and processes it in the `onNetworkReply` method.
 */
void HourlyWeather::fetchHourlyWeather() {
    // Define the URL for the OpenWeatherMap API request
    QString urlStr = "https://api.openweathermap.org/data/3.0/onecall?lat=42.9834&lon=-81.233&exclude=minutely,daily,alerts&appid=788e9dce02c51126d2cdc0c8fc52b759&units=metric";
    QUrl url(urlStr);  // Create a URL object for the API request
    QNetworkRequest request(url);  // Create a network request with the URL
    manager->get(request);  // Send the GET request using the network manager
}

/**
 * @brief Slot to handle the network reply after fetching hourly weather data.
 * @param reply The network reply containing the response from the API.
 * @details This method processes the response received from the OpenWeatherMap API. 
 *          It checks for errors, reads the response, parses the JSON data, 
 *          and emits the `hourlyWeatherFetched` signal with the parsed JSON object.
 *          If the response contains invalid JSON or an error occurs, a warning is logged.
 */
void HourlyWeather::onNetworkReply(QNetworkReply *reply) {
    // Check if the reply contains an error
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Hourly weather fetch error:" << reply->errorString();  // Log the error message
        reply->deleteLater();  // Delete the reply object to free memory
        return;
    }

    // Read the response from the reply
    QByteArray response = reply->readAll();  // Read all the data from the reply
    QJsonDocument doc = QJsonDocument::fromJson(response);  // Parse the response into a JSON document

    // Check if the parsed JSON is valid and is an object
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject jsonObj = doc.object();  // Extract the JSON object from the document
        emit hourlyWeatherFetched(jsonObj);  // Emit the signal with the parsed JSON object
    } else {
        qWarning() << "Invalid JSON received for hourly weather";  // Log a warning if the JSON is invalid
    }

    reply->deleteLater();  // Delete the reply object to free memory
}
