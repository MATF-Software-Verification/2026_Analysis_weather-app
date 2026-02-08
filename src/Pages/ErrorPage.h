#ifndef ERRORPAGE_H
#define ERRORPAGE_H

#include "Page.h"

#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class ErrorPage : public Page
{
    Q_OBJECT

public:
    ErrorPage(QWidget *parent = nullptr);
    ~ErrorPage() = default;

public slots:
    void setErrMsg(const QString &errMsg);

    // todo widgets scroll area like in DetailedWeatherPage
    virtual void addNewWidget(const QSharedPointer<WeatherData>){}
    virtual void addErrorWidget(const QString&){}

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *upperLayout;
    QLabel *errorLabel;
    QPushButton *homeButton;
};

#endif // ERRORPAGE_H
