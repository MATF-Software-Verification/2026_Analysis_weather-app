#include "DetailedWeatherPage.h"

#include "MainWindow.h"
#include "WeatherData.h"
#include "ErrorWidget.h"
#include "WeatherWidget.h"
#include "GeoLocationData.h"
#include "Settings.h"
#include "DetailedWeatherData.h"

#include <iostream>
#include <QStackedWidget>
#include <QTimer>
#include <QTransform>
#include <QFrame>

DetailedWeatherPage::DetailedWeatherPage(QWidget *parent)
    : Page{parent}
    , api(new DetailedWeatherAPI(this))
    , mainLayout(new QHBoxLayout(this))
    , widgetsScrollArea(new QScrollArea())
    , weatherScrollArea(new QScrollArea())
    , widgetsScrollAreaContents(new QWidget())
    , weatherScrollAreaContents(new QWidget())
    , widgetsLayout(new QGridLayout())
    , weatherLayout(new QVBoxLayout())
    , buttonsLayout(new QHBoxLayout())
    , returnToHomePage(new QPushButton("< Home"))
    , horizontalSpacer(new QSpacerItem(spacerWidth, 0, QSizePolicy::Expanding, QSizePolicy::Minimum))
    , addToSavedLocations(new QPushButton("Add"))
    , scrollTimer(new QTimer(this))
    , locationInfo(new LocationInfoWidget(this))
    , basicInfo(new BasicInfoWidget(this))
    , humidityUvRain(new HumidityUvRainWidget(this))
    , visibilityPressureSnow(new VisibilityPressureSnowWidget(this))
    , windInfo(new WindInfoWidget(this))
    , hourlyWidget(new HourlyWeatherWidget(this))
    , dailyWidget(new DailyWeatherWidget(this))
    , selectedWidget(nullptr)
    , minmaxWidget(new MinMaxTempWidget(this))
    , sunWidget(new SunWidget(this))
{
    widgetsScrollAreaContents->setLayout(widgetsLayout);
    widgetsLayout->setAlignment(Qt::AlignTop);

    widgetsScrollArea->setWidget(widgetsScrollAreaContents);
    widgetsScrollArea->setWidgetResizable(true);
    widgetsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    buttonsLayout->addWidget(returnToHomePage);
    buttonsLayout->addItem(horizontalSpacer);
    buttonsLayout->addWidget(addToSavedLocations);

    weatherLayout->setAlignment(Qt::AlignTop);
    weatherLayout->addLayout(buttonsLayout);

    weatherLayout->addWidget(locationInfo);
    weatherLayout->addWidget(basicInfo);
    basicInfo->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    weatherLayout->addWidget(minmaxWidget);
    weatherLayout->addWidget(sunWidget);
    weatherLayout->addWidget(humidityUvRain);
    weatherLayout->addWidget(visibilityPressureSnow);
    weatherLayout->addWidget(windInfo);
    weatherLayout->addWidget(hourlyWidget);
    weatherLayout->addWidget(dailyWidget);

    weatherScrollAreaContents->setLayout(weatherLayout);
    weatherScrollArea->setWidget(weatherScrollAreaContents);
    weatherScrollArea->setWidgetResizable(true);
    weatherScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addWidget(widgetsScrollArea);
    mainLayout->addWidget(weatherScrollArea);

    connect(returnToHomePage, &QPushButton::clicked, this->mainWindow, &MainWindow::showHomePage);
    connect(returnToHomePage, &QPushButton::clicked, this, &DetailedWeatherPage::homeButtonClicked);
    connect(addToSavedLocations, &QPushButton::clicked, this, &DetailedWeatherPage::addButtonClicked);
    connect(this, &DetailedWeatherPage::locationSaved, this->mainWindow, &MainWindow::getLocationData);
    connect(api, &DetailedWeatherAPI::errorOccurred, mainWindow, &MainWindow::showErrorPage);
    connect(this, &DetailedWeatherPage::detailedDataFetched, mainWindow, &MainWindow::showDetailedWeatherPage);
}

void DetailedWeatherPage::addNewWidget(const QSharedPointer<WeatherData> weatherData)
{
    auto *widget = new WeatherWidget(weatherData, widgetsScrollAreaContents);
    connect(widget, &WeatherWidget::clicked, this, &DetailedWeatherPage::getData);

    widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    widget->setMaximumWidth(widgetsScrollArea->viewport()->width());

    int position = static_cast<int>(Settings::instance().savedLocations().indexOf(widget->data->location()));

    if(Settings::instance().shareLocation()){
        position++;
    }

    position == -1 ? widgetsLayout->addWidget(widget, 0, 0, 1, 1) // User location widget
                   : widgetsLayout->addWidget(widget, position, 0, 1, 1);

    m_widgets.emplaceBack(widget);

    if(mainWindow->currentPage() == this){
        QTimer::singleShot(100, this, &DetailedWeatherPage::highlightWidget);
    }
}

void DetailedWeatherPage::addErrorWidget(const QString &errMsg)
{
    auto *widget = new ErrorWidget(errMsg);
    widgetsLayout->addWidget(widget, 0, 0, 1, 1);
    m_widgets.emplaceBack(widget);
}

void DetailedWeatherPage::getData(const GeoLocationData &data)
{
    bool showAddbutton = data.getRenamedPlace() != "My location" &&
                         Settings::instance().savedLocations().indexOf(data) == -1;

    showAddbutton ? this->addToSavedLocations->setVisible(true)
                  : this->addToSavedLocations->setVisible(false);

    api->fetchData(data);
    connect(api, &DetailedWeatherAPI::dataFetched, this, &DetailedWeatherPage::setData);
}

void DetailedWeatherPage::setData(const QSharedPointer<DetailedWeatherData> detailedData){
    this->data = detailedData;
    highlightWidget();

    locationInfo->updateData(this->data->location().getCoordinates(),
                             this->data->location().getRenamedPlace(),
                             this->data->location().getDetailedPlace());

    basicInfo->updateData(this->data->weatherCode(), this->data->isDay(), this->data->timezone(),
                          this->data->temperature(), this->data->apparentTemperature());

    minmaxWidget->updateData(this->data->weeklyMaxTemp(), this->data->weeklyMinTemp(),
                             this->data->weeklyDayName());

    sunWidget->updateData(this->data->timezone(), this->data->sunrise(), this->data->sunset());

    hourlyWidget->updateData(this->data->hourlyTemperature(), this->data->hourlyCode(),
                             this->data->hourlyIsDay(), this->data->hourlyTimeStamp());

    dailyWidget->updateData(this->data->weeklyDayName(), this->data->weeklyCode(),
                            this->data->weeklyMinTemp(), this->data->weeklyMaxTemp(),
                            this->data->fullHourlyTemperature());

    humidityUvRain->updateData(this->data->humidity(), this->data->uvIndex(), this->data->precipitation());
    visibilityPressureSnow->updateData(this->data->visibility(), this->data->pressure(), this->data->snowDepth());
    windInfo->updateData(this->data->windSpeed(), this->data->windGusts(), this->data->windDirection());

    emit detailedDataFetched();
}

void DetailedWeatherPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    int newWidth = this->width() / 3;
    widgetsScrollArea->setFixedWidth(newWidth);
}

void DetailedWeatherPage::addButtonClicked()
{
    emit locationSaved(this->data->location());
    this->addToSavedLocations->setVisible(false);
    Settings::instance().savedLocations().push_back(this->data->location());
    weatherScrollArea->ensureWidgetVisible(locationInfo);
}

void DetailedWeatherPage::homeButtonClicked()
{
    if(selectedWidget != nullptr){
        selectedWidget->resetHighlight();
    }
    selectedWidget = nullptr;
}

void DetailedWeatherPage::highlightWidget()
{
    if(this->data == nullptr){
        return;
    }

    if(selectedWidget != nullptr){
        selectedWidget->resetHighlight();
    }

    auto newSelectedWidget = std::find_if(m_widgets.begin(), m_widgets.end(), [this](const auto* element) {
        const auto* widget = dynamic_cast<const WeatherWidget*>(element); // Check if it's not an ErrorWidget
        return (widget != nullptr ? widget->data->location() == this->data->location() : false);
    });

    if (newSelectedWidget != m_widgets.end()) {
        auto *newSelectedWeatherWidget = dynamic_cast<WeatherWidget*>(*newSelectedWidget);
        selectedWidget = newSelectedWeatherWidget;
        widgetsScrollArea->ensureWidgetVisible(selectedWidget);
        selectedWidget->setHighlight();
    }
}
