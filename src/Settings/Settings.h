#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QMap>

#include "Serializable.h"

class GeoLocationData;

class Settings : public QObject, public Serializable
{
    Q_OBJECT

public:
    static Settings& instance();

    QString temperatureUnitApiParameter() const;
    QString windSpeedUnitApiParameter() const;
    QString precipitationUnitApiParameter() const;

    QString temperatureUnitString() const;
    QString windSpeedUnitString() const;
    QString precipitationUnitString() const;
    QString visibilityUnitString() const;

    QString temperatureUnitName() const;
    QString windSpeedUnitName() const;
    QString precipitationUnitName() const;

    QVariant toVariant() const override;
    void fromVariant(const QVariant & variant) override;
    QString weatherCodeToIcon(const int weatherCode, const bool isDay);

    enum TemperatureUnit
    {
        CELSIUS,
        FAHRENHEIT
    };
    Q_ENUM(TemperatureUnit)

    enum WindSpeedUnit
    {
        KMH,
        MPH,
        MS,
        KNOTS
    };
    Q_ENUM(WindSpeedUnit)

    enum PrecipitationUnit
    {
        MILLIMETRES,
        INCHES
    };
    Q_ENUM(PrecipitationUnit)

    inline QList<GeoLocationData>& savedLocations()
    {
        return m_savedLocations;
    }

    inline bool shareLocation() const
    {
        return m_shareLocation;
    }

    inline Settings::TemperatureUnit tempUnit() const
    {
        return m_temperatureUnit;
    }

private:
    Settings();
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    QString temperatureUnitToApiParameter(const TemperatureUnit tempUnit) const;
    QString windSpeedUnitToApiParameter(const WindSpeedUnit windSpeedUnit) const;
    QString precipitationUnitToApiParameter(const PrecipitationUnit precUnit) const;

    QString temperatureUnitToString(const TemperatureUnit tempUnit) const;
    QString windSpeedUnitToString(const WindSpeedUnit windSpeedUnit) const;
    QString precipitationUnitToString(const PrecipitationUnit precUnit) const;
    QString visibilityUnitToString(const PrecipitationUnit unit) const; // Depends on precipitation unit in the API

    QString temperatureUnitsNames(const TemperatureUnit tempUnit) const;
    QString windSpeedUnitsNames(const WindSpeedUnit windSpeedUnit) const;
    QString precipitationUnitsNames(const PrecipitationUnit precUnit) const;

    Settings::TemperatureUnit m_temperatureUnit = TemperatureUnit::CELSIUS;
    Settings::WindSpeedUnit m_windSpeedUnit = WindSpeedUnit::KMH;
    Settings::PrecipitationUnit m_precipitationUnit = PrecipitationUnit::MILLIMETRES;

    bool m_shareLocation = false;
    QList<GeoLocationData> m_savedLocations;

    friend class SettingsDialog;
};

#endif // SETTINGS_H
