#ifndef GEOCODINGAPI_H
#define GEOCODINGAPI_H

#include "ApiHandler.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QObject>

#include "GeoLocationData.h"

class GeocodingAPI : public ApiHandler
{
    Q_OBJECT
public:
    GeocodingAPI();
    ~GeocodingAPI() = default;

    void replyFinished(QNetworkReply* reply) override;

signals:
    void geocodingDataUpdated(const QList<GeoLocationData> &locations);

public slots:
    void geocodeCity(const QString &location);

private:
    QString OPEN_CAGE_API_KEY;
};

#endif // GEOCODINGAPI_H
