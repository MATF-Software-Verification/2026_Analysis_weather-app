#include "HomePage.h"

#include <QShortcut>
#include <QCompleter>
#include <QStringListModel>

#include "MainWindow.h"
#include "WeatherData.h"
#include "ErrorWidget.h"
#include "WeatherWidget.h"
#include "SettingsDialog.h"
#include "Settings.h"

HomePage::HomePage(QWidget *parent)
    : Page{parent}
    , mainLayout(new QVBoxLayout(this))
    , upperLayout(new QHBoxLayout())
    , searchBar(new QLineEdit())
    , settingsButton(new QPushButton())
    , refreshButton(new QPushButton())
    , scrollArea(new QScrollArea())
    , scrollAreaContents(new QWidget())
    , widgetsLayout(new QGridLayout())
    , completer(new CustomCompleter(this))
    , debounceTimer(new QTimer(this))
    , settingsPixmap(settingsIconPath)
    , settingsIcon(settingsPixmap)
    , refreshPixmap(refreshIconPath)
    , refreshIcon(refreshPixmap)
{
    refreshButton->setIcon(refreshIcon);
    upperLayout->addWidget(refreshButton);

    searchBar->setPlaceholderText("Enter location...");
    searchBar->setCompleter(completer);
    upperLayout->addWidget(searchBar);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    debounceTimer->setInterval(timerInterval);
    debounceTimer->setSingleShot(true);

    settingsButton->setIcon(settingsIcon);
    upperLayout->addWidget(settingsButton);
    mainLayout->addLayout(upperLayout);

    widgetsLayout->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);
    widgetsLayout->setAlignment(Qt::AlignTop);
    widgetsLayout->setColumnStretch(0, 1);
    widgetsLayout->setColumnStretch(1, 1);
    scrollAreaContents->setLayout(widgetsLayout);
    scrollArea->setWidget(scrollAreaContents);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea);

    styleSheetsSetup();

    connect(settingsButton, &QPushButton::clicked, this, &HomePage::openSettingsDialog);
    connect(refreshButton, &QPushButton::clicked, mainWindow, &MainWindow::refreshPages);
    connect(&geocodingApi, &GeocodingAPI::geocodingDataUpdated, this, &HomePage::updateCompleter);
    connect(completer, QOverload<const QString&>::of(&QCompleter::activated), this, &HomePage::onCompletionActivated);
    connect(searchBar, &QLineEdit::textChanged, this, [this]() { debounceTimer->start(); });
    connect(debounceTimer, &QTimer::timeout, this, &HomePage::onSearchBarTextChanged);
    connect(this, &HomePage::searchBarPressed, &geocodingApi, &GeocodingAPI::geocodeCity);
    connect(this, &HomePage::locationObjectSelected, mainWindow, &MainWindow::getDetailedData);
}

HomePage::~HomePage()
{
    delete completer->model();
}

void HomePage::addNewWidget(const QSharedPointer<WeatherData> data)
{
    auto *widget = new WeatherWidget(data, scrollAreaContents);
    connect(widget, &WeatherWidget::clicked, this->mainWindow, &MainWindow::getDetailedData);

    int position = static_cast<int>(Settings::instance().savedLocations().indexOf(widget->data->location()));

    if(Settings::instance().shareLocation()){
        position++;
    }

    position == -1 ? widgetsLayout->addWidget(widget, 0, 0, 1, 1) // User location widget
                   : widgetsLayout->addWidget(widget, position / 2, position % 2, 1, 1);

    widget->setFixedHeight(widgetHeight);

    m_widgets.emplaceBack(widget);
}

void HomePage::addErrorWidget(const QString &errMsg)
{
    auto *widget = new ErrorWidget(errMsg);
    widgetsLayout->addWidget(widget, 0, 0, 1, 1);
    widget->setFixedHeight(widgetHeight);
    m_widgets.emplaceBack(widget);
}

void HomePage::openSettingsDialog()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}

void HomePage::onSearchBarTextChanged()
{
    emit searchBarPressed(searchBar->text());
}

void HomePage::updateCompleter(const QList<GeoLocationData>& locations)
{
    this->locations = locations;
    QStringList places;
    for (const auto& location : locations) {
        places.append(location.getDetailedPlace());
    }

    if (completer->model() != nullptr) {
        delete completer->model();
    }
    completer->setModel(new QStringListModel(places, completer));
    completer->complete();
}

void HomePage::onCompletionActivated(const QString& text)
{
    for (const auto& location : locations) {
        if (location.getDetailedPlace() == text) {
            emit locationObjectSelected(location);
            break;
        }
    }
    searchBar->clear();
    delete completer->model();
    completer->setModel(new QStringListModel());
    completer->complete();
}

void HomePage::styleSheetsSetup()
{
    searchBar->setStyleSheet(
        "QLineEdit {"
        "    border: 3px solid gray;"
        "    border-radius: 4px;"
        "    padding: 0 8px;"
        "    background-color: rgb(28, 28, 28);"
        "    selection-background-color: darkgray;"
        "    color: white;"
        "}"
        "QLineEdit:focus {"
        "    border-color: rgb(28, 28, 28);"
        "    background-color: rgb(40, 40, 40);"
        "}"
        );

    scrollArea->setStyleSheet(
        "QScrollBar:vertical {"
        "    width: 7px;"
        "    border-radius: 30px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: gray;"
        "    min-height: 20px;"
        "    border-radius: 30px;"
        "}"
        );

    settingsButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #d7d6d5;"
        "}"
        );

    refreshButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #d7d6d5;"
        "}"
        );
}
