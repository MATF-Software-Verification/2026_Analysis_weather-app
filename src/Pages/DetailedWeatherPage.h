#ifndef DETAILEDWEATHERPAGE_H
#define DETAILEDWEATHERPAGE_H

#include "Page.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QPushButton>
#include <QSharedPointer>
#include <QScrollBar>
#include <QTimer>
#include <QLabel>
#include <QTimeZone>

#include "GeoLocationData.h"
#include "DetailedPageWidgets.h"
#include "DetailedWeatherAPI.h"

class WeatherData;
class MainWindow;
class WeatherWidget;
class GeoLocationData;
class DetailedWeatherData;

class DetailedWeatherPage : public Page
{
    Q_OBJECT

public:
    explicit DetailedWeatherPage(QWidget *parent = nullptr);
    ~DetailedWeatherPage() = default;

    void resizeEvent(QResizeEvent *event) override;

    QSharedPointer<DetailedWeatherData> data;
    DetailedWeatherAPI* api;

public slots:
    void addNewWidget(const QSharedPointer<WeatherData> data) override;
    void addErrorWidget(const QString &errMsg) override;
    void getData(const GeoLocationData &data);
    void setData(const QSharedPointer<DetailedWeatherData> data);

private slots:
    void addButtonClicked();
    void homeButtonClicked();

signals:
    void locationSaved(const GeoLocationData &data);
    void errorOccurred(const QString &errMsg);
    void detailedDataFetched();

private:
    static const int spacerWidth = 40;

    QHBoxLayout *mainLayout;
    QScrollArea *widgetsScrollArea;
    QScrollArea *weatherScrollArea;
    QWidget *widgetsScrollAreaContents;
    QWidget *weatherScrollAreaContents;
    QGridLayout *widgetsLayout;
    QVBoxLayout *weatherLayout;
    QHBoxLayout *buttonsLayout;
    QPushButton *returnToHomePage;
    QSpacerItem *horizontalSpacer;
    QPushButton *addToSavedLocations;
    QTimer *scrollTimer;
    LocationInfoWidget *locationInfo;
    BasicInfoWidget *basicInfo;
    HumidityUvRainWidget *humidityUvRain;
    VisibilityPressureSnowWidget *visibilityPressureSnow;
    WindInfoWidget *windInfo;
    HourlyWeatherWidget *hourlyWidget;
    DailyWeatherWidget *dailyWidget;
    WeatherWidget *selectedWidget;
    MinMaxTempWidget *minmaxWidget;
    SunWidget *sunWidget;

    void highlightWidget();
};

#endif // DETAILEDWEATHERPAGE_H
