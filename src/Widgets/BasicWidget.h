#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QWidget>

#include <QHBoxLayout>

class BasicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicWidget(QWidget *parent = nullptr);

protected:
    QHBoxLayout *mainLayout;
    const QString fontName = "Roboto";
};

#endif // BASICWIDGET_H
