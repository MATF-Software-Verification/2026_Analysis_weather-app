#include "DetailedWeatherData.h"

#include <iostream>

DetailedWeatherData::DetailedWeatherData(const GeoLocationData &location,
                                         const int temperature,
                                         const int weatherCode,
                                         const bool isDay,
                                         const QTimeZone &timezone,
                                         const int windSpeed,
                                         const int windGusts,
                                         const int windDirection,
                                         const int apparentTemperature,
                                         const int precipitation,
                                         const double snowDepth,
                                         const int uvIndex,
                                         const int humidity,
                                         const int visibility,
                                         const int pressure,
                                         const QVector<int> &hourlyTemperature,
                                         const QVector<int> &fullHourlyTemperature,
                                         const QVector<int> &hourlyCode,
                                         const QVector<bool> &hourlyIsDay,
                                         const QVector<QString> &hourlyTimeStamp,
                                         const QVector<int> &weeklyMaxTemp,
                                         const QVector<int> &weeklyMinTemp,
                                         const QVector<int> &weeklyCode,
                                         const QVector<QString> &sunrise,
                                         const QVector<QString> &sunset,
                                         const QVector<QString> &weeklyDayName)
    : WeatherData(location, temperature, weeklyMaxTemp[0], weeklyMinTemp[0], weatherCode, isDay, timezone)
    , m_location(location)
    , m_temperature(temperature)
    , m_weatherCode(weatherCode)
    , m_isDay(isDay)
    , m_timezone(timezone)
    , m_windSpeed(windSpeed)
    , m_windGusts(windGusts)
    , m_windDirection(windDirection)
    , m_apparentTemperature(apparentTemperature)
    , m_precipitation(precipitation)
    , m_snowDepth(snowDepth)
    , m_uvIndex(uvIndex)
    , m_humidity(humidity)
    , m_visibility(visibility)
    , m_pressure(pressure)
    , m_hourlyTemperature(hourlyTemperature)
    , m_fullHourlyTemperature(fullHourlyTemperature)
    , m_hourlyCode(hourlyCode)
    , m_hourlyIsDay(hourlyIsDay)
    , m_hourlyTimeStamp(hourlyTimeStamp)
    , m_weeklyMaxTemp(weeklyMaxTemp)
    , m_weeklyMinTemp(weeklyMinTemp)
    , m_weeklyCode(weeklyCode)
    , m_sunrise(sunrise)
    , m_sunset(sunset)
    , m_weeklyDayName(weeklyDayName)
{}
