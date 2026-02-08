#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include "BasicWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

class WeatherData;
class GeoLocationData;

class WeatherWidget : public BasicWidget
{
    Q_OBJECT

public:
    explicit WeatherWidget(const QSharedPointer<WeatherData> data, QWidget *parent = nullptr);
    ~WeatherWidget() = default;
    void mousePressEvent(QMouseEvent *event) override;

    void setHighlight();
    void resetHighlight();

    const QSharedPointer<WeatherData> data;

signals:
    void clicked(const GeoLocationData &data);

protected:
    void resizeEvent(QResizeEvent*) override;

private:
    static const int hBoxSpacing = 10;
    static const int hBoxMarginSize = 10;
    static const int locationFontSize = 17;
    static const int countryFontSize = 11;
    static const int timeFontSize = 14;
    static const int temperatureFontSize = 45;
    static const int minmaxTemperatureFontSize = 14;
    static const int iconWidth = 70;
    static const int iconHeight = 70;
    static const int labelHeight = 40;

    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *cityCountryLayout;
    QLabel* temperatureLabel;
    QLabel* locationLabel;
    QLabel* countryLabel;
    QLabel* maxTemperatureLabel;
    QLabel* minTemperatureLabel;
    QLabel* timeLabel;
    QLabel* iconLabel;
    QPixmap weatherIcon;

    void adjustLabelFontSize(QLabel* label, const int widthConstraint, const int heightConstraint, const int initialSize);
};

#endif // WEATHERWIDGET_H
