#include "BasicWidget.h"

BasicWidget::BasicWidget(QWidget *parent)
    : QWidget{parent}
    , mainLayout(new QHBoxLayout(this))
{
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("BasicWidget { border-radius: 20px; background-color: #598be0; }");
}
