#include "SettingsDialog.h"

#include <QListView>
#include <QListWidget>
#include <QMetaObject>
#include <QMetaEnum>
#include <QDebug>

#include "MainWindow.h"
#include "HomePage.h"
#include "WidgetsManager.h"
#include "GeoLocationData.h"
#include "Serializer.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , settings(Settings::instance())
    , mainLayout(new QVBoxLayout(this))
    , locationSwitch(new QCheckBox("Share Location", this))
    , temperatureUnit(new QComboBox(this))
    , windSpeedUnit(new QComboBox(this))
    , precipitationUnit(new QComboBox(this))
    , listWidget(new WidgetsManager(this))
    , buttonLayout(new QHBoxLayout())
    , save(new QPushButton("Save"))
    , cancel(new QPushButton("Cancel"))
    , widgetOrder(settings.savedLocations())
    , trashCan("../Resources/trashCan/redTrash.png")
    , orderCan("../Resources/order/order.png")
    , trashIcon(trashCan)
    , orderIcon(orderCan)
{
    setWindowTitle("Settings");

    locationSwitch->setChecked(settings.m_shareLocation);
    mainLayout->addWidget(locationSwitch);

    const QMetaObject &meta = Settings::staticMetaObject;

    QMetaEnum temperatureMetaEnum = meta.enumerator(meta.indexOfEnumerator("TemperatureUnit"));
    const int tempKeyCount = temperatureMetaEnum.keyCount();

    for (int i = 0; i < tempKeyCount; ++i) {
        int enumValue = temperatureMetaEnum.keyToValue(temperatureMetaEnum.key(i));
        auto tempUnit = static_cast<Settings::TemperatureUnit>(enumValue);

        temperatureUnit->addItem(settings.temperatureUnitsNames(tempUnit), QVariant::fromValue(tempUnit));
    }
    temperatureUnit->setCurrentText(settings.temperatureUnitName());

    QMetaEnum windSpeedMetaEnum = meta.enumerator(meta.indexOfEnumerator("WindSpeedUnit"));
    const int windKeyCount = windSpeedMetaEnum.keyCount();

    for (int i = 0; i < windKeyCount; ++i) {
        int enumValue = windSpeedMetaEnum.keyToValue(windSpeedMetaEnum.key(i));
        auto windUnit = static_cast<Settings::WindSpeedUnit>(enumValue);

        windSpeedUnit->addItem(settings.windSpeedUnitsNames(windUnit), QVariant::fromValue(windUnit));
    }
    windSpeedUnit->setCurrentText(settings.windSpeedUnitName());

    QMetaEnum precipitationMetaEnum = meta.enumerator(meta.indexOfEnumerator("PrecipitationUnit"));
    const int precKeyCount = precipitationMetaEnum.keyCount();

    for (int i = 0; i < precKeyCount; ++i) {
        int enumValue = precipitationMetaEnum.keyToValue(precipitationMetaEnum.key(i));
        auto precipUnit = static_cast<Settings::PrecipitationUnit>(enumValue);

        precipitationUnit->addItem(settings.precipitationUnitsNames(precipUnit),
                                   QVariant::fromValue(precipUnit));
    }
    precipitationUnit->setCurrentText(settings.precipitationUnitName());

    mainLayout->addWidget(temperatureUnit);
    mainLayout->addWidget(windSpeedUnit);
    mainLayout->addWidget(precipitationUnit);

    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    listWidget->installEventFilter(this);


    for (const auto& location : settings.savedLocations()) {

        auto *listItem = new QListWidgetItem();
        listItem->setData(Qt::UserRole, location.toVariant());
        listWidget->addItem(listItem);

        auto *customWidget = new QWidget();
        auto *layout = new QHBoxLayout(customWidget);
        auto *orderLabel = new QLabel();
        orderLabel->setPixmap(orderIcon.pixmap(orderIcon.actualSize(QSize(20, 20))));
        auto *locationNameEdit = new QLineEdit(location.getRenamedPlace());
        locationNameEdit->setStyleSheet("QLineEdit { border: none; }");
        auto *deleteButton = new QPushButton();
        deleteButton->setIcon(trashIcon);

        layout->addWidget(orderLabel);
        layout->addWidget(locationNameEdit, 1);

        layout->addWidget(deleteButton);

        connect(locationNameEdit, &QLineEdit::returnPressed, [locationNameEdit]() {
            locationNameEdit->clearFocus();
        });

        connect(deleteButton, &QPushButton::clicked, this, [this, listItem]() {
            auto locationMap = listItem->data(Qt::UserRole).toMap();
            const auto geoLocation = GeoLocationData::fromVariantMap(locationMap);
            widgetOrder.removeOne(geoLocation);

            delete listItem;
        });

        listWidget->setItemWidget(listItem, customWidget);
        listItem->setSizeHint(customWidget->minimumSizeHint());
    }

    connect(listWidget, &WidgetsManager::itemsRearranged, this, [this](){
        widgetOrder.clear();

        for (int i = 0; i < listWidget->count(); i++) {
            auto locationMap = listWidget->item(i)->data(Qt::UserRole).toMap();
            const auto geoLocation = GeoLocationData::fromVariantMap(locationMap);
            widgetOrder.append(geoLocation);
        }
    });

    mainLayout->addWidget(listWidget);

    buttonLayout->addWidget(save);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancel);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(save, &QPushButton::clicked, this, &SettingsDialog::changeSettings);
    // TODO fix grandparent
    connect(this, &SettingsDialog::settingsChanged, qobject_cast<MainWindow*>(this->parent()->parent()->parent()), &MainWindow::refreshPages);
    connect(cancel, &QPushButton::clicked, this, &SettingsDialog::resetOrder);
    connect(cancel, &QPushButton::clicked, this, &SettingsDialog::close);
}

void SettingsDialog::changeSettings()
{
    Settings::TemperatureUnit selectedTempUnit = static_cast<Settings::TemperatureUnit>(temperatureUnit->itemData(temperatureUnit->currentIndex()).toInt());
    Settings::WindSpeedUnit selectedWindUnit= static_cast<Settings::WindSpeedUnit>(windSpeedUnit->itemData(windSpeedUnit->currentIndex()).toInt());
    Settings::PrecipitationUnit selectedPrecUnit = static_cast<Settings::PrecipitationUnit>(precipitationUnit->itemData(precipitationUnit->currentIndex()).toInt());

    for (int i = 0; i < listWidget->count(); i++) {
        auto locationMap = listWidget->item(i)->data(Qt::UserRole).toMap();
        auto geoLocation = GeoLocationData::fromVariantMap(locationMap);

        QString newLocationName = listWidget->itemWidget(listWidget->item(i))->findChild<QLineEdit *>()->text();
        geoLocation.setRenamedPlace(newLocationName);

        auto iter = std::find(widgetOrder.begin(), widgetOrder.end(), geoLocation);
        if (iter != widgetOrder.end()) {
            *iter = geoLocation;
        }
    }

    settings.m_temperatureUnit = selectedTempUnit;
    settings.m_windSpeedUnit = selectedWindUnit;
    settings.m_precipitationUnit = selectedPrecUnit;
    settings.m_shareLocation = locationSwitch->isChecked();
    settings.m_savedLocations = widgetOrder;

    emit settingsChanged();
    this->close();
}

void SettingsDialog::resetOrder()
{
    widgetOrder = settings.savedLocations();
}


bool SettingsDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == listWidget && event->type() == QEvent::KeyPress) {
        auto *keyEvent = dynamic_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}
