#ifndef USERLOCATION_H
#define USERLOCATION_H

#include <QGeoPositionInfoSource>

class GeoLocationData;

class UserLocation : public QObject
{
    Q_OBJECT
public:
    explicit UserLocation(QObject *parent = nullptr);
    void getLocation();

private:
    QGeoPositionInfoSource *source;

    bool userLocationRequested = false;
    const QString errMsg = "Unable to access your location. Please check and enable location permissions in your system settings.";

private slots:
    void positionUpdated(const QGeoPositionInfo &info);
    void handleError(const QGeoPositionInfoSource::Error);

signals:
    void userLocationFetched(const GeoLocationData &data);
    void userLocationError(const QString &errMsg);
};

#endif // USERLOCATION_H
