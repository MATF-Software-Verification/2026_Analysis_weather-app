#include "ErrorWidget.h"

ErrorWidget::ErrorWidget(const QString &errorMessage, QWidget *parent)
    : BasicWidget{parent}
    , errorMessageLabel(new QLabel(errorMessage, this))
{
    mainLayout->addWidget(errorMessageLabel);
    errorMessageLabel->setWordWrap(true);
    this->setLayout(mainLayout);
}
