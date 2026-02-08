#ifndef DETAILEDWEATHERAPI_H
#define DETAILEDWEATHERAPI_H

#include "ApiHandler.h"

#include <QGeoCoordinate>
#include <QString>
#include <QNetworkAccessManager>
#include <QObject>

#include "GeoLocationData.h"

class DetailedWeatherData;

class DetailedWeatherAPI : public ApiHandler
{
    Q_OBJECT

public:
    explicit DetailedWeatherAPI(QObject *parent = nullptr);
    ~DetailedWeatherAPI() = default;
    void fetchData(const GeoLocationData &location);

private slots:
    void replyFinished(QNetworkReply *reply) override;

signals:
    void dataFetched(const QSharedPointer<DetailedWeatherData> data);

private:
    GeoLocationData location;
};

#endif // DETAILEDWEATHERAPI_H

