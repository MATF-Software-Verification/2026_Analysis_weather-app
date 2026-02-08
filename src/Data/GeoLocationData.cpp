#include "GeoLocationData.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedPointer>
#include <QVariantMap>
#include <QVariantList>

GeoLocationData::GeoLocationData(QString place, QString renamedPlace, QGeoCoordinate coordinates, QString country)
    : m_place(std::move(place))
    , m_renamedPlace(std::move(renamedPlace))
    , m_coordinates(std::move(coordinates))
    , m_country(std::move(country))
{
    qRegisterMetaType<GeoLocationData>("GeoLocationData");
}

GeoLocationData::GeoLocationData(QString renamedPlace, QGeoCoordinate coordinates)
    : m_place("")
    , m_renamedPlace(std::move(renamedPlace))
    , m_coordinates(std::move(coordinates))
    , m_country("")
{
    qRegisterMetaType<GeoLocationData>("GeoLocationData");
}

bool GeoLocationData::operator==(const GeoLocationData &other) const
{
    return this->m_coordinates == other.m_coordinates;
}

QVariant GeoLocationData::toVariant() const{
    QVariantMap map;
    map.insert("place", m_place);
    map.insert("renamedPlace", m_renamedPlace);
    map.insert("latitude", m_coordinates.latitude());
    map.insert("longitude", m_coordinates.longitude());
    map.insert("country", m_country);

    return map;
}
void GeoLocationData::fromVariant(const QVariant &variant){

    const auto map = variant.toMap();
    m_place = map.value("place").toString();
    m_renamedPlace = map.value("renamedPlace").toString();
    auto latitude = map.value("latitude").toDouble();
    auto longitude = map.value("longitude").toDouble();
    m_coordinates = QGeoCoordinate(latitude, longitude);
    m_country = map.value("country").toString();
}

GeoLocationData GeoLocationData::fromVariantMap(const QVariantMap& geoLocation)
{
    return GeoLocationData{geoLocation.value("place").toString(),
                           geoLocation.value("renamedPlace").toString(),
                           QGeoCoordinate(geoLocation.value("latitude").toDouble(),
                                          geoLocation.value("longitude").toDouble()),
                           geoLocation.value("country").toString()
    };
}

