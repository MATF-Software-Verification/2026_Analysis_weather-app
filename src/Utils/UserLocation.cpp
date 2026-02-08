#include "UserLocation.h"

#include "GeoLocationData.h"
#include "Settings.h"

UserLocation::UserLocation(QObject *parent)
    : QObject{parent}
    , source(QGeoPositionInfoSource::createDefaultSource(this))
{
    if(source != nullptr){
        connect(source, &QGeoPositionInfoSource::positionUpdated, this, &UserLocation::positionUpdated);
        connect(source, &QGeoPositionInfoSource::errorOccurred, this, &UserLocation::handleError);
    }
}

void UserLocation::getLocation()
{
    if(source == nullptr){
        emit userLocationError(errMsg);
    }
    else{
        source->requestUpdate();
    }
}

void UserLocation::positionUpdated(const QGeoPositionInfo &info)
{
    // positionUpdated signal is emitted twice (?) so we check if it's the first time
    if(!userLocationRequested){
        userLocationRequested = true;
        GeoLocationData data = GeoLocationData("My location", info.coordinate());
        emit userLocationFetched(data);
    }
}

void UserLocation::handleError(const QGeoPositionInfoSource::Error)
{
    emit userLocationError(errMsg);
}
