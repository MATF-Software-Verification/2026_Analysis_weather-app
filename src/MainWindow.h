#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QGeoPositionInfo;
class Settings;
class WeatherData;
class GeoLocationData;
class WeatherWidget;
class HomePage;
class DetailedWeatherPage;
class ErrorPage;
class UserLocation;
class GeoLocationData;
class WeatherAPI;
class Data;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget* currentPage();


public slots:
    void showHomePage();
    void showDetailedWeatherPage();
    void showErrorPage(const QString &errMsg);
    void refreshPages();
    void getLocationData(const GeoLocationData &location);
    void getDetailedData(const GeoLocationData &location);

signals:
    void detailedDataRequested(const GeoLocationData &data);
    void errorPageShown(const QString &errMsg);
    void deletePageWidgets();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    Settings &settings;
    HomePage *homePage;
    DetailedWeatherPage *detailedWeatherPage;
    ErrorPage *errorPage;
    QStackedWidget *stackedWidget;
    UserLocation *userLocation;
    WeatherAPI *weatherApi;

    void getSavedLocationsData();
    void requestUserLocationData();
    void serializeData();
};
#endif // MAINWINDOW_H
