#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    GraphDialog(const QVector<int>& temperatures, QString dayNameString, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<int> m_temperatures;
    QString m_dayName;
    int m_minTemp;
    int m_maxTemp;

    double mapTemperatureToY(int temperature);
    void calculateTemperatureRange();
};

class minMaxTempGraphDialog : public QDialog
{
    Q_OBJECT

public:
    minMaxTempGraphDialog(const QVector<int>& maxTemperatures, const QVector<int>& minTemperatures,
                          const QVector<QString> &dayNames, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QVector<int> m_maxTemperatures;
    QVector<int> m_minTemperatures;
    QVector<QString> m_dayNames;
    int m_minTemp;
    int m_maxTemp;

    void calculateTemperatureRange();
    QStringList getAbbreviatedDayNames();
};

class MapDialog : public QDialog {

    Q_OBJECT

    QLabel *mapLabel;
    QImage mapImage;
    QVBoxLayout *layout;

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MapDialog(QWidget *parent = nullptr);
    void drawCoordinateDot(double latitude, double longitude);
};

#endif // GRAPHDIALOG_H
