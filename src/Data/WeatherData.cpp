#include "WeatherData.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedPointer>

#include "GeoLocationData.h"

WeatherData::WeatherData(const GeoLocationData &location,
                         const int temperature,
                         const int highestTemperature,
                         const int lowestTemperature,
                         const int weatherCode,
                         const bool isDay,
                         QTimeZone timezone)
    : m_location(location)
    , m_temperature(temperature)
    , m_highestTemperature(highestTemperature)
    , m_lowestTemperature(lowestTemperature)
    , m_weatherCode(weatherCode)
    , m_isDay(isDay)
    , m_timezone(std::move(timezone))
{ }
