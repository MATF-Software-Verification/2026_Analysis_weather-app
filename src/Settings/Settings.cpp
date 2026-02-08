#include "Settings.h"

#include <QVariantMap>
#include <QVariantList>

#include "GeoLocationData.h"
#include "Serializer.h"

QString Settings::temperatureUnitToApiParameter(const TemperatureUnit tempUnit) const
{
    switch (tempUnit) {
    case Settings::TemperatureUnit::CELSIUS:
        return "celsius";
    case Settings::TemperatureUnit::FAHRENHEIT:
        return "fahrenheit";
    default:
        return "";
    }
}

QString Settings::windSpeedUnitToApiParameter(const WindSpeedUnit windSpeedUnit) const
{
    switch (windSpeedUnit) {
    case Settings::WindSpeedUnit::KMH:
        return "kmh";
    case Settings::WindSpeedUnit::MPH:
        return "mph";
    case Settings::WindSpeedUnit::MS:
        return "ms";
    case Settings::WindSpeedUnit::KNOTS:
        return "kn";
    default:
        return "";
    }
}

QString Settings::precipitationUnitToApiParameter(const PrecipitationUnit precUnit) const
{
    switch (precUnit) {
    case Settings::PrecipitationUnit::MILLIMETRES:
        return "mm";
    case Settings::PrecipitationUnit::INCHES:
        return "inch";
    default:
        return "";
    }
}

QString Settings::temperatureUnitToString(const TemperatureUnit tempUnit) const
{
    switch (tempUnit) {
    case Settings::TemperatureUnit::CELSIUS:
        return "°C";
    case Settings::TemperatureUnit::FAHRENHEIT:
        return "°F";
    default:
        return "";
    }
}

QString Settings::windSpeedUnitToString(const WindSpeedUnit windSpeedUnit) const
{
    switch (windSpeedUnit) {
    case Settings::WindSpeedUnit::KMH:
        return "km/h";
    case Settings::WindSpeedUnit::MPH:
        return "mph";
    case Settings::WindSpeedUnit::MS:
        return "m/s";
    case Settings::WindSpeedUnit::KNOTS:
        return "kn";
    default:
        return "";
    }
}

QString Settings::precipitationUnitToString(const PrecipitationUnit precUnit) const
{
    switch (precUnit) {
    case Settings::PrecipitationUnit::MILLIMETRES:
        return "mm";
    case Settings::PrecipitationUnit::INCHES:
        return "in";
    default:
        return "";
    }
}

// Depends on precipitation unit in the API
QString Settings::visibilityUnitToString(const PrecipitationUnit unit) const
{
    switch (unit) {
    case Settings::PrecipitationUnit::MILLIMETRES:
        return "m";
    case Settings::PrecipitationUnit::INCHES:
        return "ft";
    default:
        return "";
    }
}

QString Settings::temperatureUnitsNames(const TemperatureUnit tempUnit) const
{
    switch (tempUnit) {
    case Settings::TemperatureUnit::CELSIUS:
        return "Celsius";
    case Settings::TemperatureUnit::FAHRENHEIT:
        return "Fahrenheit";
    default:
        return "";
    }
}

QString Settings::windSpeedUnitsNames(const WindSpeedUnit windSpeedUnit) const
{
    switch (windSpeedUnit) {
    case Settings::WindSpeedUnit::KMH:
        return "Kilometres per hour";
    case Settings::WindSpeedUnit::MPH:
        return "Miles per hour";
    case Settings::WindSpeedUnit::MS:
        return "Metres per second";
    case Settings::WindSpeedUnit::KNOTS:
        return "Knots";
    default:
        return "";
    }
}

QString Settings::precipitationUnitsNames(const PrecipitationUnit precUnit) const
{
    switch (precUnit) {
    case Settings::PrecipitationUnit::MILLIMETRES:
        return "Millimetres";
    case Settings::PrecipitationUnit::INCHES:
        return "Inches";
    default:
        return "";
    }
}

Settings::Settings()
{
    Serializer::load(*this, "../Serialization/settings.json");
}

Settings& Settings::instance()
{
    static Settings instance;
    return instance;
}

QVariant Settings::toVariant() const
{
    QVariantMap map;
    map.insert("shareLocation", m_shareLocation);
    map.insert("temperatureUnit", m_temperatureUnit);
    map.insert("windSpeedUnit", m_windSpeedUnit);
    map.insert("precipitationUnit", m_precipitationUnit);
    QVariantList locations;
    for (const auto &savedLocation : m_savedLocations){
        QVariantMap location = savedLocation.toVariant().toMap();
        locations.append(location);
    }
    map.insert("locations", locations);

    return map;
}

void Settings::fromVariant(const QVariant &variant)
{
    const auto map = variant.toMap();
    m_shareLocation = map.value("shareLocation").toBool();
    m_temperatureUnit = map.value("temperatureUnit").value<Settings::TemperatureUnit>();
    m_windSpeedUnit = map.value("precipitationUnit").value<Settings::WindSpeedUnit>();
    m_precipitationUnit = map.value("precipitationUnit").value<Settings::PrecipitationUnit>();

    const auto locations = map.value("locations").toList();
    for(const auto& location : locations){
        const auto geoLocation = location.toMap();
        m_savedLocations.append(GeoLocationData::fromVariantMap(geoLocation));
    }
}

QString Settings::temperatureUnitApiParameter() const
{
    return temperatureUnitToApiParameter(m_temperatureUnit);
}

QString Settings::windSpeedUnitApiParameter() const
{
    return windSpeedUnitToApiParameter(m_windSpeedUnit);
}

QString Settings::precipitationUnitApiParameter() const
{
    return precipitationUnitToApiParameter(m_precipitationUnit);
}

QString Settings::temperatureUnitString() const
{
    return temperatureUnitToString(m_temperatureUnit);
}

QString Settings::windSpeedUnitString() const
{
    return windSpeedUnitToString(m_windSpeedUnit);
}

QString Settings::precipitationUnitString() const
{
    return precipitationUnitToString(m_precipitationUnit);
}

QString Settings::visibilityUnitString() const
{
    // Depends on precipitation unit in the API
    return visibilityUnitToString(m_precipitationUnit);
}

QString Settings::temperatureUnitName() const
{
    return temperatureUnitsNames(m_temperatureUnit);
}

QString Settings::windSpeedUnitName() const
{
    return windSpeedUnitsNames(m_windSpeedUnit);
}

QString Settings::precipitationUnitName() const
{
    return precipitationUnitsNames(m_precipitationUnit);
}

QString Settings::weatherCodeToIcon(const int weatherCode, const bool isDay)
{
    const QString iconBasePath = "../Resources/weatherIcons/";

    switch (weatherCode) {
    case 0:
        return isDay ? iconBasePath + "Sunny.png" : iconBasePath + "Clear.png";
    case 1:
        return isDay ? iconBasePath + "PartlyCloudyDay.png" : iconBasePath + "PartlyCloudyNight.png";
    case 2:
        return iconBasePath + "Cloudy.png";
    case 3:
        return iconBasePath + "Overcast.png";
    case 45:
        return iconBasePath + "Fog.png";
    case 48:
        return iconBasePath + "FreezingFog.png";
    case 51:
    case 53:
    case 55:
        return iconBasePath + "ModRain.png";
    case 56:
    case 57:
        return iconBasePath + "FreezingDrizzle.png";
    case 61:
        return iconBasePath + "ModRain.png";
    case 63:
        return iconBasePath + "HeavyRain.png";
    case 65:
        return isDay ? iconBasePath + "HeavyRainSwrsDay.png" : iconBasePath + "HeavyRainSwrsNight.png";
    case 66:
    case 67:
        return iconBasePath + "FreezingRain.png";
    case 71:
        return iconBasePath + "ModSnow.png";
    case 73:
        return iconBasePath + "HeavySnow.png";
    case 75:
        return isDay ? iconBasePath + "HeavySnowSwrsDay.png" : iconBasePath + "HeavySnowSwrsNight.png";
    case 77:
        return isDay ? iconBasePath + "IsoSnowSwrsDay.png" : iconBasePath + "IsoSnowSwrsNight.png";
    case 80:
        return isDay ? iconBasePath + "IsoRainSwrsDay.png" : iconBasePath + "IsoRainSwrsNight.png";
    case 81:
        return isDay ? iconBasePath + "ModRainSwrsDay.png" : iconBasePath + "ModRainSwrsNight.png";
    case 82:
        return isDay ? iconBasePath + "HeavyRainSwrsDay.png" : iconBasePath + "HeavyRainSwrsNight.png";
    case 85:
        return isDay ? iconBasePath + "IsoSnowSwrsDay.png" : iconBasePath + "IsoSnowSwrsNight.png";
    case 86:
        return isDay ? iconBasePath + "HeavySnowSwrsDay.png" : iconBasePath + "HeavySnowSwrsNight.png";
    case 95:
        return isDay ? iconBasePath + "PartCloudRainThunderDay.png" : iconBasePath + "PartCloudRainThunderNight.png";
    case 96:
    case 99:
        return isDay ? iconBasePath + "PartCloudSleetSnowThunderDay.png" : iconBasePath + "PartCloudSleetSnowThunderNight.png";
    default:
        return iconBasePath + "Cloudy.png";
    }
}
