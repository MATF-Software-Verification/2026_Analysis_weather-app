#include "GraphDialog.h"

#include <algorithm>

#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>

GraphDialog::GraphDialog(const QVector<int>& temperatures, QString dayNameString, QWidget *parent)
    : QDialog(parent)
    , m_temperatures(temperatures)
    , m_dayName(std::move(dayNameString))
{
    calculateTemperatureRange();
    resize(400, 300);
    setWindowTitle(m_dayName + " Hourly Temperature");
}

void GraphDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    update();
}

minMaxTempGraphDialog::minMaxTempGraphDialog(const QVector<int>& maxTemperatures,
                                             const QVector<int>& minTemperatures,
                                             const QVector<QString>& dayNames, QWidget *parent)
    : QDialog(parent)
    , m_maxTemperatures(maxTemperatures)
    , m_minTemperatures(minTemperatures)
    , m_dayNames(dayNames)
{
    calculateTemperatureRange();
    resize(400, 300);
    setWindowTitle("Weekly Highest/Lowest Temperature");
}

void minMaxTempGraphDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    update();
}

MapDialog::MapDialog(QWidget *parent)
    : QDialog(parent)
    , mapLabel(new QLabel(this))
    , layout(new QVBoxLayout(this))
{
    layout->addWidget(mapLabel);
    mapLabel->setScaledContents(true);

    mapImage.load("../Resources/map/map.jpg");
    mapLabel->setPixmap(QPixmap::fromImage(mapImage.scaled(mapImage.width(), mapImage.height(), Qt::KeepAspectRatio)));

    setWindowTitle(tr("Map"));
    resize(mapImage.width(), mapImage.height());
}

void MapDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    update();
}

void MapDialog::drawCoordinateDot(double latitude, double longitude) {
    const int mapWidth = mapImage.width();
    const int mapHeight = mapImage.height();

    int xCoordinate = static_cast<int>((longitude + 180.0) / 360.0 * mapWidth);
    int yCoordinate = static_cast<int>((90.0 - latitude) / 180.0 * mapHeight);

    QPainter painter(&mapImage);
    painter.setPen(QPen(Qt::red, 4));
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPointF(xCoordinate, yCoordinate), 2, 2);
    mapLabel->setPixmap(QPixmap::fromImage(mapImage));
}

void minMaxTempGraphDialog::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int leftMargin = 60;
    const int rightMargin = 50;
    const int topMargin = 10;
    const int bottomMargin = 50;
    QRect drawingRect = rect().adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);

    QPen gridPen(QColor(70, 70, 70), 1);
    QPen labelPen(Qt::white);
    QStringList dayLabels = getAbbreviatedDayNames();
    const int numDays = static_cast<int>(dayLabels.size());
    const int yRange = m_maxTemp - m_minTemp + 6;
    const double yStep = static_cast<double>(drawingRect.height()) / yRange;

    painter.setPen(gridPen);
    for (int i = 0; i < numDays; i++) {
        int x = drawingRect.left() + (drawingRect.width() * i / (numDays - 1));
        painter.drawLine(x, drawingRect.top(), x, drawingRect.bottom());
        painter.setPen(labelPen);
        painter.drawText(x - painter.fontMetrics().horizontalAdvance(dayLabels[i]) / 2,
                         drawingRect.bottom() + 20, dayLabels[i]);
        painter.setPen(gridPen);
    }

    for (int i = m_minTemp - 3; i <= m_maxTemp + 3; i += 2) {
        int y = drawingRect.bottom() - static_cast<int>((i - m_minTemp + 3) * yStep);
        painter.drawLine(drawingRect.left(), y, drawingRect.right(), y);
        painter.setPen(labelPen);
        painter.drawText(drawingRect.left() - painter.fontMetrics().horizontalAdvance(QString::number(i) + "°") - 10,
                             y + painter.fontMetrics().height() / 4, QString::number(i) + "°");
            painter.setPen(gridPen);
    }

    auto mapDayToX = [&drawingRect, numDays](int dayIndex) -> int {
        return drawingRect.left() + (drawingRect.width() * dayIndex / (numDays - 1));
    };
    auto mapTemperatureToY = [&drawingRect, this, yStep](int temperature) -> int {
        return drawingRect.bottom() - static_cast<int>((temperature - m_minTemp + 3) * yStep);
    };

    painter.setPen(QPen(Qt::red, 3));
    for (int i = 1; i < m_maxTemperatures.size(); i++) {
        painter.drawLine(mapDayToX(i - 1), mapTemperatureToY(m_maxTemperatures[i - 1]),
                         mapDayToX(i), mapTemperatureToY(m_maxTemperatures[i]));
    }
    painter.setPen(QPen(Qt::blue, 3));
    for (int i = 1; i < m_minTemperatures.size(); i++) {
        painter.drawLine(mapDayToX(i - 1), mapTemperatureToY(m_minTemperatures[i - 1]),
                         mapDayToX(i), mapTemperatureToY(m_minTemperatures[i]));
    }
}

QStringList minMaxTempGraphDialog::getAbbreviatedDayNames() {
    QStringList dayList;
    for (const QString &day : m_dayNames) {
        dayList << day.mid(0, 3);
    }
    return dayList;
}

void GraphDialog::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int leftMargin = 60;
    int rightMargin = 10;
    int topMargin = 10;
    int bottomMargin = 50;
    QRect drawingRect = rect().adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);

    QPen gridPen(QColor(70, 70, 70), 1);
    painter.setPen(gridPen);

    QStringList timeLabels = {"00:00", "06:00", "12:00", "18:00"};
    QVector<int> hours = {0, 6, 12, 18, 24};
    for (int i : hours) {
        int x = drawingRect.left() + (drawingRect.width() * i / 24);
        painter.drawLine(x, drawingRect.top(), x, drawingRect.bottom());
    }

    double yRange = m_maxTemp - m_minTemp + 6;
    double yStep = drawingRect.height() / yRange;
    int startLabel = m_minTemp - 3;

    for (int i = startLabel; i <= m_maxTemp + 3; i += 2) {
        int y = static_cast<int>(drawingRect.bottom() - ((i - (m_minTemp - 3)) * yStep));
        painter.drawLine(drawingRect.left(), y, drawingRect.right(), y);
    }

    gridPen.setColor(QColor(240, 240, 240));
    painter.setPen(gridPen);

    int labelStep = drawingRect.width() / 23;
    for (int i = 0; i < 4; ++i) {
        int x = drawingRect.left() + hours[i] * labelStep;
        painter.drawText(x - painter.fontMetrics().horizontalAdvance(timeLabels[i]) / 2,
                         rect().bottom() - 10, timeLabels[i]);
    }

    for (int i = m_minTemp - 3; i <= m_maxTemp + 3; i += 2) {
        int y = static_cast<int>(drawingRect.bottom() - ((i - (m_minTemp - 3)) * yStep));
        QString label = QString::number(i) + "°";
        int labelWidth = painter.fontMetrics().horizontalAdvance(label);

        painter.drawText(drawingRect.left() - labelWidth - 10,
                         y + painter.fontMetrics().height() / 4, label);
    }

    QPen linePen(Qt::blue, 2);
    painter.setPen(linePen);
    QPainterPath path;

    double xScale = static_cast<double>(drawingRect.width()) / 23;
    double yScale = static_cast<double>(drawingRect.height()) / yRange;

    QPointF firstPoint(drawingRect.left(), drawingRect.bottom() - ((m_temperatures.first() - (m_minTemp - 3)) * yScale));
    path.moveTo(firstPoint);

    for (int i = 1; i < m_temperatures.size(); ++i) {
        double x = drawingRect.left() + i * xScale;
        double y = drawingRect.bottom() - ((m_temperatures[i] - (m_minTemp - 3)) * yScale);
        path.lineTo(QPointF(x, y));
    }

    path.lineTo(QPointF(drawingRect.right(), drawingRect.bottom()));
    path.lineTo(QPointF(drawingRect.left(), drawingRect.bottom()));
    path.closeSubpath();

    QBrush fillBrush(QColor(0, 0, 255, 127));
    painter.fillPath(path, fillBrush);

    painter.setPen(linePen);
    painter.drawPath(path);
}

void minMaxTempGraphDialog::calculateTemperatureRange()
{
    m_minTemp = *std::min_element(m_minTemperatures.constBegin(), m_minTemperatures.constEnd());
    m_maxTemp = *std::max_element(m_maxTemperatures.constBegin(), m_maxTemperatures.constEnd());
}

void GraphDialog::calculateTemperatureRange()
{
    m_minTemp = *std::min_element(m_temperatures.constBegin(), m_temperatures.constEnd());
    m_maxTemp = *std::max_element(m_temperatures.constBegin(), m_temperatures.constEnd());
}

double GraphDialog::mapTemperatureToY(int temperature)
{
    double range = m_maxTemp - m_minTemp;
    if (range == 0) {
        return rect().height() / 2.0;
    }
    double scale = rect().height() / range;
    return rect().bottom() - (temperature - m_minTemp) * scale;
}
