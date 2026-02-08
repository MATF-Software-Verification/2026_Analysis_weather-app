#ifndef GEOLOCATIONDATA_H
#define GEOLOCATIONDATA_H

#include "Data.h"
#include "Serializable.h"

#include <QGeoCoordinate>

class GeoLocationData : public Data, public Serializable
{
public:
    GeoLocationData() = default;
    ~GeoLocationData() = default;
    GeoLocationData(const GeoLocationData &other) = default;
    GeoLocationData(QString place, QString renamedPlace, QGeoCoordinate coordinates, QString country);
    GeoLocationData(QString renamedPlace, QGeoCoordinate coordinates);

    inline QString getDetailedPlace() const{
        return m_place;
    }

    inline QString getRenamedPlace() const{
        return m_renamedPlace;
    }

    inline QString getCountry() const{
        return m_country;
    }

    inline QGeoCoordinate getCoordinates() const{
        return m_coordinates;
    }

    inline void setRenamedPlace(QString &renamedPlace){
        m_renamedPlace = renamedPlace;
    }

    QVariant toVariant() const override;
    void fromVariant(const QVariant & variant) override;
    static GeoLocationData fromVariantMap(const QVariantMap& geoLocation);

    GeoLocationData& operator= (const GeoLocationData& other) = default;
    bool operator== (const GeoLocationData &other) const;

private:
    QString m_place;
    QString m_renamedPlace;
    QGeoCoordinate m_coordinates;
    QString m_country;
};
Q_DECLARE_METATYPE(GeoLocationData)

#endif // GEOLOCATIONDATA_H
