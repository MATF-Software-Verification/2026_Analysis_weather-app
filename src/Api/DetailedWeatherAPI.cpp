#include "DetailedWeatherAPI.h"

#include <iostream>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDate>

#include "DetailedWeatherData.h"
#include "ApiHandler.h"
#include "Parser.h"
#include "Settings.h"

DetailedWeatherAPI::DetailedWeatherAPI(QObject *parent)
    : ApiHandler(parent)
{
    connect(networkManager, &QNetworkAccessManager::finished, this, &DetailedWeatherAPI::replyFinished);
}

void DetailedWeatherAPI::fetchData(const GeoLocationData &location)
{
    QString latitude  = QString::number(location.getCoordinates().latitude());
    QString longitude = QString::number(location.getCoordinates().longitude());

    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", latitude);
    query.addQueryItem("longitude", longitude);
    query.addQueryItem("current", "temperature_2m,weather_code,is_day,wind_speed_10m,apparent_temperature,snow_depth,relative_humidity_2m,visibility,pressure_msl,uv_index,wind_direction_10m,wind_gusts_10m");
    query.addQueryItem("daily", "temperature_2m_max,temperature_2m_min,weather_code,sunrise,sunset,precipitation_sum");
    query.addQueryItem("timezone", "auto");

    query.addQueryItem("hourly", "temperature_2m,weather_code,is_day,precipitation");

    query.addQueryItem("temperature_unit", Settings::instance().temperatureUnitApiParameter());
    query.addQueryItem("wind_speed_unit", Settings::instance().windSpeedUnitApiParameter());
    query.addQueryItem("precipitation_unit", Settings::instance().precipitationUnitApiParameter());

    url.setQuery(query);
    QNetworkRequest request(url);
    auto *reply = networkManager->get(request);
    reply->setProperty("location", location.toVariant());

    connect(reply, &QNetworkReply::errorOccurred, this, [this](){
        emit errorOccurred(networkErrMsg);
    });
}

void DetailedWeatherAPI::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(networkErrMsg);
        return;
    }

    GeoLocationData location;
    location.fromVariant(reply->property("location"));

    QString jsonData = reply->readAll();
    auto data = Parser::parseDetailedWeatherData(jsonData, location);

    if(data.isNull()){
        emit errorOccurred(parseErrMsg);
    }
    else{
        emit dataFetched(data);
    }

    reply->deleteLater();
}
