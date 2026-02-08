#include "GeocodingAPI.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QList>
#include <QCoreApplication>
#include <QSettings>

#include "Parser.h"

GeocodingAPI::GeocodingAPI()
{
    const QString configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    const QSettings settings(configFilePath, QSettings::IniFormat);
    OPEN_CAGE_API_KEY = settings.value("API/Key").toString();
    connect(networkManager, &QNetworkAccessManager::finished, this, &GeocodingAPI::replyFinished);
}

void GeocodingAPI::geocodeCity(const QString& location)
{
    QString apiUrl = QString("https://api.opencagedata.com/geocode/v1/json?q=%1&key=%2")
                    .arg(location, OPEN_CAGE_API_KEY);

    QNetworkRequest request{QUrl(apiUrl)};
    networkManager->get(request);
}

void GeocodingAPI::replyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

    if (!jsonResponse.isObject()) {
        return;
    }

    QJsonObject jsonObject = jsonResponse.object();
    if (!jsonObject.contains("results") || !jsonObject["results"].isArray()) {
        return;
    }

    QJsonArray resultsArray = jsonObject["results"].toArray();
    if (resultsArray.isEmpty()) {
        return;
    }

    // json za https://api.opencagedata.com/geocode/v1/json?q=Be&key=0741d020f58441f6b58ae4dc4128740d formatted

    QList<GeoLocationData> locations;
    locations.reserve(resultsArray.size());

    Parser::parseGeocodingData(resultsArray, locations);

    emit geocodingDataUpdated(locations);
    reply->deleteLater();
}
