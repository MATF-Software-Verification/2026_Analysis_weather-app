#include "WeatherWidget.h"

#include <QString>
#include <QLabel>
#include <QTime>
#include <QApplication>
#include <QFont>
#include <QPalette>
#include <QFrame>
#include <QMargins>
#include <QLineEdit>

#include "WeatherData.h"
#include "Settings.h"

WeatherWidget::WeatherWidget(const QSharedPointer<WeatherData> data, QWidget *parent)
    : BasicWidget{parent}
    , data(data)
    , leftLayout(new QVBoxLayout())
    , rightLayout(new QVBoxLayout())
    , cityCountryLayout(new QVBoxLayout())
    , temperatureLabel(new QLabel(QString::number(data->temperature()) + "°", this))
    , locationLabel(new QLabel(data->location().getRenamedPlace().toUpper(), this))
    , countryLabel(new QLabel(data->location().getCountry(), this))
    , maxTemperatureLabel(new QLabel("H:" + QString::number(data->highestTemperature()) + "°", this))
    , minTemperatureLabel(new QLabel("L:" + QString::number(data->lowestTemperature()) + "°", this))
    , timeLabel(new QLabel(QDateTime::currentDateTime().toTimeZone(data->timezone()).toString("HH:mm"), this))
    , iconLabel(new QLabel(this))
    , weatherIcon(Settings::instance().weatherCodeToIcon(data->weatherCode(), data->isDay()))
{
    mainLayout->setSpacing(hBoxSpacing);
    mainLayout->setContentsMargins(hBoxMarginSize, hBoxMarginSize, hBoxMarginSize, hBoxMarginSize);

    locationLabel->setFont(QFont(fontName, locationFontSize, QFont::Bold));
    countryLabel->setFont(QFont(fontName, countryFontSize, QFont::Normal));
    temperatureLabel->setFont(QFont(fontName, temperatureFontSize, QFont::Bold));
    maxTemperatureLabel->setFont(QFont(fontName, minmaxTemperatureFontSize, QFont::Normal));
    minTemperatureLabel->setFont(QFont(fontName, minmaxTemperatureFontSize, QFont::Normal));
    iconLabel->setPixmap(weatherIcon.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedHeight(iconHeight);

    locationLabel->setStyleSheet("color: white;");
    locationLabel->setWordWrap(true);
    timeLabel->setStyleSheet("color: white;");
    temperatureLabel->setStyleSheet("color: white;");
    countryLabel->setStyleSheet("color: white;");
    countryLabel->setWordWrap(true);

    locationLabel->setAlignment(Qt::AlignCenter);
    countryLabel->setAlignment(Qt::AlignCenter);
    cityCountryLayout->addWidget(locationLabel, 0, Qt::AlignCenter);
    cityCountryLayout->addWidget(countryLabel, 0, Qt::AlignCenter);
    cityCountryLayout->setSpacing(0);
    cityCountryLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->addLayout(cityCountryLayout);
    leftLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    leftLayout->addWidget(timeLabel, 0, Qt::AlignCenter);

    rightLayout->addWidget(maxTemperatureLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(temperatureLabel, 0, Qt::AlignCenter);
    rightLayout->addWidget(minTemperatureLabel, 0, Qt::AlignCenter);

    auto* separator = new QFrame();
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("color: black;");

    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(separator);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void WeatherWidget::adjustLabelFontSize(QLabel* label, const int widthConstraint, const int heightConstraint, const int initialSize) {
    QFont font = label->font();
    int minFontSize = 5;
    int currentFontSize = initialSize;

    font.setPointSize(currentFontSize);
    label->setFont(font);
    label->setWordWrap(true);

    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(0, 0, widthConstraint, QWIDGETSIZE_MAX,
                                     Qt::TextWordWrap | Qt::AlignCenter, label->text());

    while ((textRect.width() > widthConstraint || textRect.height() > heightConstraint) && currentFontSize > minFontSize) {
        currentFontSize--;
        font.setPointSize(currentFontSize);
        fm = QFontMetrics(font);
        textRect = fm.boundingRect(0, 0, widthConstraint, QWIDGETSIZE_MAX,
                                   Qt::TextWordWrap | Qt::AlignCenter, label->text());
    }

    label->setFont(font);
    textRect = fm.boundingRect(0, 0, widthConstraint, QWIDGETSIZE_MAX,
                               Qt::TextWordWrap | Qt::AlignCenter, label->text());

    label->setFixedSize(textRect.width(), textRect.height());
}

void WeatherWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    int margins = mainLayout->contentsMargins().left() + mainLayout->contentsMargins().right();
    int spacing = mainLayout->spacing();
    int maxWidth = this->width() / 2 - margins - spacing;

    adjustLabelFontSize(locationLabel, maxWidth, labelHeight, locationFontSize);
    adjustLabelFontSize(countryLabel, maxWidth, labelHeight, countryFontSize);
    adjustLabelFontSize(temperatureLabel, maxWidth, iconHeight, temperatureFontSize);
}

void WeatherWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit clicked(data->location());
}

void WeatherWidget::setHighlight()
{
    setStyleSheet("WeatherWidget { border-radius: 20px; background-color: #0848F3; }");
}

void WeatherWidget::resetHighlight()
{
    setStyleSheet("WeatherWidget { border-radius: 20px; background-color: #598be0; }");
}
