#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Data;

class ApiHandler : public QObject
{
    Q_OBJECT

public:
    explicit ApiHandler(QObject *parent = nullptr);
    virtual ~ApiHandler();

signals:
    void errorOccurred(const QString &errMsg);

protected slots:
    virtual void replyFinished(QNetworkReply* reply) = 0;

protected:
    QNetworkAccessManager* networkManager;

    const QString networkErrMsg = "Error occured while fetching weather data. "
                                  "Please check your internet connection.";

    const QString parseErrMsg = "Error occured while parsing API response,";
};

#endif // APIHANDLER_H
