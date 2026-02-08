#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include "BasicWidget.h"

#include <QLabel>

class ErrorWidget : public BasicWidget
{
public:
    explicit ErrorWidget(const QString &errorMessage, QWidget *parent = nullptr);

private:
    QLabel *errorMessageLabel;

};

#endif // ERRORWIDGET_H
