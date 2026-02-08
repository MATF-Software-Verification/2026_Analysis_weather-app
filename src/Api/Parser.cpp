#include "Parser.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "WeatherData.h"
#include "DetailedWeatherData.h"
#include "GeoLocationData.h"

WeatherData* Parser::parseWeatherData(const QString &jsonData, const GeoLocationData &geoLocation) {
    try {
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        if (doc.isNull()) {
            throw std::runtime_error("Invalid JSON data");
        }

        QJsonObject obj = doc.object();
        if (!obj.contains("timezone") || !obj.contains("current") || !obj.contains("daily")) {
            throw std::runtime_error("Missing required JSON fields");
        }

        QString timezoneId = obj.value("timezone").toString();
        QTimeZone timeZone = QTimeZone(timezoneId.toLatin1());
        if (!timeZone.isValid()) {
            throw std::runtime_error("Invalid timezone ID");
        }

        QJsonObject current = obj.value("current").toObject();
        int temperature = static_cast<int>(qRound(current.value("temperature_2m").toDouble()));
        int weatherCode = current.value("weather_code").toInt();
        bool isDay = static_cast<bool>(current.value("is_day").toInt());

        QJsonObject daily = obj.value("daily").toObject();
        QJsonArray dailyMaxTemperature = daily.value("temperature_2m_max").toArray();
        QJsonArray dailyMinTemperature = daily.value("temperature_2m_min").toArray();

        if (dailyMaxTemperature.isEmpty() || dailyMinTemperature.isEmpty()) {
            throw std::runtime_error("Daily temperature arrays are empty");
        }

        int maxTemperature = static_cast<int>(qRound(dailyMaxTemperature[0].toDouble()));
        int minTemperature = static_cast<int>(qRound(dailyMinTemperature[0].toDouble()));

        return new WeatherData(geoLocation,
                               temperature,
                               maxTemperature,
                               minTemperature,
                               weatherCode,
                               isDay,
                               timeZone);
    }
    catch (const std::exception &e) {
        return nullptr;
    }
}

QSharedPointer<DetailedWeatherData> Parser::parseDetailedWeatherData(const QString& jsonData, const GeoLocationData &geoLocation)
{
    try{
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj = doc.object();
        QString timezoneId = obj.value("timezone").toString();
        QJsonObject current = obj.value("current").toObject();
        QJsonObject daily = obj.value("daily").toObject();
        QJsonObject hourly = obj.value("hourly").toObject();
        QTimeZone timeZone = QTimeZone(timezoneId.toLatin1());
        int temperature = static_cast<int>(qRound(current.value("temperature_2m").toDouble()));
        int weatherCode = current.value("weather_code").toInt();
        bool isDay = static_cast<bool>(current.value("is_day").toInt());
        int windSpeed = static_cast<int>(qRound(current.value("wind_speed_10m").toDouble()));
        int apparentTemperature = static_cast<int>(qRound(current.value("apparent_temperature").toDouble()));
        double snowDepth = current.value("snow_depth").toDouble();
        int humidity = static_cast<int>(qRound(current.value("relative_humidity_2m").toDouble()));
        int visibility = static_cast<int>(qRound(current.value("visibility").toDouble()));
        int pressure =  static_cast<int>(qRound(current.value("pressure_msl").toDouble()));
        int uvIndex = static_cast<int>(qRound(current.value("uv_index").toDouble()));
        int windDirection = static_cast<int>(qRound(current.value("wind_direction_10m").toDouble()));
        int windGusts = static_cast<int>(qRound(current.value("wind_gusts_10m").toDouble()));

        QJsonArray hourlyTimeJ = hourly.value("time").toArray();
        QDateTime currentTime = QDateTime::currentDateTime().toTimeZone(timeZone);
        QString formattedCurrentTime = currentTime.toString(Qt::ISODate).left(16);
        int matchingIndex = -1;
        const int hourlyTimeJsize = static_cast<int>(hourlyTimeJ.size());
        for (int i = 0; i < hourlyTimeJsize; ++i) {
            QString timeEntry = hourlyTimeJ[i].toString();
            if (formattedCurrentTime < timeEntry) {
                matchingIndex = i - 1;
                break;
            }
        }

        QJsonArray hourlyTempJ = hourly.value("temperature_2m").toArray();
        QJsonArray hourlyCodeJ = hourly.value("weather_code").toArray();
        QJsonArray hourlyIsDayJ = hourly.value("is_day").toArray();

        QVector<int> hourlyTemperatures;
        hourlyTemperatures.reserve(hourlyTempJ.size());

        std::transform(hourlyTempJ.begin(), hourlyTempJ.end(), std::back_inserter(hourlyTemperatures),
                       [](const QJsonValue &value) {
                           return static_cast<int>(qRound(value.toDouble()));
                       });

        QVector<int> ht;
        QVector<int> hc;
        QVector<bool> hd;
        QVector<QString> hts;

        for (int i = matchingIndex, end = matchingIndex + 24; i < end; i++){
            int hourlyTemperature = static_cast<int>(qRound(hourlyTempJ[i].toDouble()));
            ht.push_back(hourlyTemperature);
            int code = hourlyCodeJ[i].toInt();
            hc.push_back(code);
            bool day = static_cast<bool>(hourlyIsDayJ[i].toInt());
            hd.push_back(day);
            QString time = hourlyTimeJ[i].toString();
            hts.push_back(time.mid(11, 5));
        }

        QJsonArray weeklyCodeJ = daily.value("weather_code").toArray();
        QJsonArray weeklySunriseJ = daily.value("sunrise").toArray();
        QJsonArray weeklySunsetJ = daily.value("sunset").toArray();
        QJsonArray weeklyDayJ = daily.value("time").toArray();
        QJsonArray weeklyPrecJ = daily.value("precipitation_sum").toArray();
        QJsonArray weeklyMaxTempJ = daily.value("temperature_2m_max").toArray();
        QJsonArray weeklyMinTempJ = daily.value("temperature_2m_min").toArray();

        QVector<int> weeklyCode;
        QVector<QString> weeklyDayName;
        QVector<int> weeklyMaxTemp;
        QVector<int> weeklyMinTemp;
        QVector<QString> weeklySunrise;
        QVector<QString> weeklySunset;

        int precipitation = static_cast<int>(qRound(weeklyPrecJ[0].toDouble()));

        for (int i = 0; i < 7; i++){
            int wc = static_cast<int>(qRound(weeklyCodeJ[i].toDouble()));
            weeklyCode.push_back(wc);

            QString dayName = QDate::fromString(static_cast<QString>(weeklyDayJ[i].toString()), "yyyy-MM-dd").toString("dddd");
            weeklyDayName.push_back(dayName);

            int dailyMaxTemperature = static_cast<int>(qRound(weeklyMaxTempJ[i].toDouble()));
            weeklyMaxTemp.push_back(dailyMaxTemperature);

            int dailyMinTemperature = static_cast<int>(qRound(weeklyMinTempJ[i].toDouble()));
            weeklyMinTemp.push_back(dailyMinTemperature);

            QString sunrise = static_cast<QString>(weeklySunriseJ[i].toString());
            weeklySunrise.push_back(sunrise);

            QString sunset = static_cast<QString>(weeklySunsetJ[i].toString());
            weeklySunset.push_back(sunset);
        }

        QSharedPointer<DetailedWeatherData> data(new DetailedWeatherData(geoLocation,
                                                                         temperature,
                                                                         weatherCode,
                                                                         isDay,
                                                                         timeZone,
                                                                         windSpeed,
                                                                         windGusts,
                                                                         windDirection,
                                                                         apparentTemperature,
                                                                         precipitation,
                                                                         snowDepth,
                                                                         uvIndex,
                                                                         humidity,
                                                                         visibility,
                                                                         pressure,
                                                                         ht,
                                                                         hourlyTemperatures,
                                                                         hc,
                                                                         hd,
                                                                         hts,
                                                                         weeklyMaxTemp,
                                                                         weeklyMinTemp,
                                                                         weeklyCode,
                                                                         weeklySunrise,
                                                                         weeklySunset,
                                                                         weeklyDayName));
        return data;
    }
    catch(const std::exception&){
        return nullptr;
    }
}

void Parser::parseGeocodingData(const QJsonArray &resultsArray, QList<GeoLocationData> &locations)
{
    for (auto resultValue : resultsArray) {
        QJsonObject resultObject = resultValue.toObject();

        if (!resultObject.contains("formatted") || !resultObject["formatted"].isString()) {
            continue;
        }

        QString place = resultObject["formatted"].toString();
        if(!place.isEmpty() && place.at(0).isDigit()){
            continue;
        }

        if (!resultObject.contains("geometry") || !resultObject["geometry"].isObject()) {
            continue;
        }

        QJsonObject geometryObject = resultObject["geometry"].toObject();
        if (!geometryObject.contains("lat") || !geometryObject.contains("lng")) {
            continue;
        }
        double latitude = geometryObject["lat"].toDouble();
        double longitude = geometryObject["lng"].toDouble();

        QString country = "";
        if(resultObject.contains("components") && resultObject["components"].isObject()){
            QJsonObject componentsObject = resultObject["components"].toObject();
            if(componentsObject.contains("country") && componentsObject["country"].isString()){
                country = componentsObject["country"].toString();
            }
        }

        QString renamedPlace;
        auto commaIndex=place.indexOf(',');
        commaIndex == -1 ? renamedPlace = place : renamedPlace = place.left(commaIndex).trimmed();

        locations.emplace_back(place, renamedPlace, QGeoCoordinate(latitude,longitude),country);
    }
}
