#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include "Data.h"

#include <QString>
#include <QTimeZone>

#include "GeoLocationData.h"

class WeatherData : public Data
{
public:
    WeatherData(const GeoLocationData &location,
                const int temperature,
                const int highestTemperature,
                const int lowestTemperature,
                const int weatherCode,
                const bool isDay,
                QTimeZone timezone);

    inline GeoLocationData location() const
    {
        return m_location;
    }

    inline int temperature() const
    {
        return m_temperature;
    }

    inline int highestTemperature() const
    {
        return m_highestTemperature;
    }

    inline int lowestTemperature() const
    {
        return m_lowestTemperature;
    }

    inline int weatherCode() const
    {
        return m_weatherCode;
    }

    inline bool isDay() const
    {
        return m_isDay;
    }

    inline QTimeZone timezone() const
    {
        return m_timezone;
    }

private:
    GeoLocationData m_location;
    int m_temperature;
    int m_highestTemperature;
    int m_lowestTemperature;
    int m_weatherCode;
    bool m_isDay;
    QTimeZone m_timezone;
};

#endif // WEATHERDATA_H
