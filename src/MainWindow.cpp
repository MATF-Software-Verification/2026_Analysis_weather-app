#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QStackedWidget>
#include <QGeoPositionInfoSource>

#include "ApiHandler.h"
#include "WeatherAPI.h"
#include "WeatherData.h"
#include "WeatherWidget.h"
#include "HomePage.h"
#include "DetailedWeatherPage.h"
#include "ErrorPage.h"
#include "GeocodingAPI.h"
#include "UserLocation.h"
#include "Settings.h"
#include "GeoLocationData.h"
#include "Serializer.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(Settings::instance())
    , homePage(new HomePage(this))
    , detailedWeatherPage(new DetailedWeatherPage(this))
    , errorPage(new ErrorPage(this))
    , stackedWidget(new QStackedWidget(this))
    , userLocation(new UserLocation(this))
    , weatherApi(new WeatherAPI(this))
{
    ui->setupUi(this);
    resize(900,600);

    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(detailedWeatherPage);
    stackedWidget->addWidget(errorPage);
    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(homePage);

    requestUserLocationData();
    getSavedLocationsData();

    connect(this, &MainWindow::errorPageShown, errorPage, &ErrorPage::setErrMsg);
    connect(this, &MainWindow::detailedDataRequested, detailedWeatherPage, &DetailedWeatherPage::getData);
    connect(this, &MainWindow::deletePageWidgets, homePage, &Page::deleteWidgets);
    connect(this, &MainWindow::deletePageWidgets, detailedWeatherPage, &Page::deleteWidgets);
    connect(userLocation, &UserLocation::userLocationFetched, this, &MainWindow::getLocationData);
    connect(userLocation, &UserLocation::userLocationError, homePage, &HomePage::addErrorWidget);
    connect(weatherApi, &WeatherAPI::errorOccurred, homePage, &HomePage::addErrorWidget);
    connect(weatherApi, &WeatherAPI::dataFetched, homePage, &HomePage::addNewWidget);
    connect(weatherApi, &WeatherAPI::dataFetched, detailedWeatherPage, &DetailedWeatherPage::addNewWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestUserLocationData()
{
    if(settings.shareLocation()){
        userLocation->getLocation();
    }
}

void MainWindow::getSavedLocationsData()
{
    for(const auto& location : settings.savedLocations()){
        getLocationData(location);
    }
}

void MainWindow::getLocationData(const GeoLocationData &location)
{
    weatherApi->fetchData(location);
}

void MainWindow::getDetailedData(const GeoLocationData &location)
{
    // todo: without this old text stays in the search bar
    stackedWidget->setCurrentWidget(detailedWeatherPage);
    stackedWidget->setCurrentWidget(homePage);

    emit detailedDataRequested(location);
}

void MainWindow::showHomePage()
{
    stackedWidget->setCurrentWidget(homePage);
}

void MainWindow::showDetailedWeatherPage()
{
    stackedWidget->setCurrentWidget(detailedWeatherPage);
}

void MainWindow::showErrorPage(const QString &errMsg)
{
    stackedWidget->setCurrentWidget(errorPage);
    emit errorPageShown(errMsg);
}

QWidget* MainWindow::currentPage()
{
    return stackedWidget->currentWidget();
}

void MainWindow::refreshPages()
{
    emit deletePageWidgets();

    // QGeoPositionInfoSource doesn't emit signals after the first error (?) so we reinitialize it
    delete userLocation;
    userLocation = new UserLocation(this);
    connect(userLocation, &UserLocation::userLocationError, homePage, &HomePage::addErrorWidget);
    connect(userLocation, &UserLocation::userLocationFetched, this, &MainWindow::getLocationData);

    requestUserLocationData();
    getSavedLocationsData();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    serializeData();
    QMainWindow::closeEvent(event);
}

void MainWindow::serializeData()
{
    Serializer::save(settings, "../Serialization/settings.json");
}
