#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSharedPointer>
#include <QCompleter>
#include <QTimer>
#include <QPushButton>
#include "Page.h"
#include "GeocodingAPI.h"
#include "Settings.h"
#include "GeoLocationData.h"
#include "CustomCompleter.h"

class WeatherWidget;
class WeatherData;
class MainWindow;

class HomePage : public Page
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

signals:
    void searchBarPressed(const QString& location);
    void locationObjectSelected(const GeoLocationData& locationData);

public slots:
    void addNewWidget(const QSharedPointer<WeatherData> data) override;
    void addErrorWidget(const QString &errMsg) override;
    void onSearchBarTextChanged();

private:
    static const int leftMargin = 25;
    static const int rightMargin = 25;
    static const int topMargin = 0;
    static const int bottomMargin = 0;
    static const int timerInterval = 200;
    static const int widgetHeight = 180;

    const QString settingsIconPath = "../Resources/settingsIcon/settings.png";
    const QString refreshIconPath = "../Resources/refresh_icon/refresh_icon.png";

    QVBoxLayout *mainLayout;
    QHBoxLayout *upperLayout;
    QLineEdit *searchBar;
    QPushButton *settingsButton;
    QPushButton *refreshButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaContents;
    QGridLayout *widgetsLayout;
    CustomCompleter *completer;
    QTimer *debounceTimer;
    QPixmap settingsPixmap;
    QIcon settingsIcon;
    QPixmap refreshPixmap;
    QIcon refreshIcon;

    QList<GeoLocationData> locations;

    void openSettingsDialog();
    void updateCompleter(const QList<GeoLocationData>& locations);
    void onCompletionActivated(const QString& text);
    void styleSheetsSetup();

    GeocodingAPI geocodingApi;
};

#endif // HOMEPAGE_H
