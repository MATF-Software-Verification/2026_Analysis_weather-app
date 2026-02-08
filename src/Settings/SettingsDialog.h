#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include "Settings.h"

class GeoLocationData;
class MainWindow;
class WidgetsManager;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() = default;

signals:
    void settingsChanged();

private slots:
    void changeSettings();
    void resetOrder();

private:
    Settings &settings;
    QVBoxLayout *mainLayout;
    QCheckBox *locationSwitch;
    QComboBox *temperatureUnit;
    QComboBox *windSpeedUnit;
    QComboBox *precipitationUnit;
    WidgetsManager *listWidget;
    QHBoxLayout *buttonLayout;
    QPushButton *save;
    QPushButton *cancel;
    QList<GeoLocationData> widgetOrder;
    QPixmap trashCan;
    QPixmap orderCan;
    QIcon trashIcon;
    QIcon orderIcon;

    void setupUi();
    bool eventFilter(QObject *obj, QEvent *event) override;

    const QString trashIconPath = "../Resources/trashCan/redTrash.png";
};

#endif // SETTINGSDIALOG_H
