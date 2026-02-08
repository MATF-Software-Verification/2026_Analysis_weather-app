#ifndef WIDGETSMANAGER_H
#define WIDGETSMANAGER_H

#include <QListWidget>

class WidgetsManager : public QListWidget
{
    Q_OBJECT

public:
    using QListWidget::QListWidget;

protected:
    void dropEvent(QDropEvent *event) override {
        QListWidget::dropEvent(event);
        emit itemsRearranged();
    }

signals:
    void itemsRearranged();
};

#endif // WIDGETSMANAGER_H
