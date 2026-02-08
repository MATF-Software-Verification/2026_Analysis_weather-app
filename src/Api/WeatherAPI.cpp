#include "WeatherAPI.h"

#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "WeatherData.h"
#include "GeoLocationData.h"
#include "Parser.h"
#include "Settings.h"

WeatherAPI::WeatherAPI(QObject *parent)
    : ApiHandler{parent}
{
    connect(networkManager, &QNetworkAccessManager::finished, this, &WeatherAPI::replyFinished);
}

void WeatherAPI::fetchData(const GeoLocationData &location)
{
    QString latitude  = QString::number(location.getCoordinates().latitude());
    QString longitude = QString::number(location.getCoordinates().longitude());

    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", latitude);
    query.addQueryItem("longitude", longitude);
    query.addQueryItem("current", "temperature_2m,weather_code,is_day");
    query.addQueryItem("daily", "temperature_2m_max,temperature_2m_min");
    query.addQueryItem("timezone", "auto");
    query.addQueryItem("temperature_unit", Settings::instance().temperatureUnitApiParameter());

    url.setQuery(query);
    QNetworkRequest request(url);
    auto *reply = networkManager->get(request);
    reply->setProperty("location", location.toVariant());

    connect(reply, &QNetworkReply::errorOccurred, this, [this](){
        emit errorOccurred(networkErrMsg);
    });
}

void WeatherAPI::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
//        qDebug() << reply->errorString().toStdString();
        emit errorOccurred(networkErrMsg);
        return;
    }

    GeoLocationData location;
    location.fromVariant(reply->property("location"));

    QString jsonData = reply->readAll();

    QSharedPointer<WeatherData> data(Parser::parseWeatherData(jsonData, location));

    if(data.isNull()){
        emit errorOccurred(parseErrMsg);
    }
    else{
        emit dataFetched(data);
    }

    reply->deleteLater();
}
