#include "Page.h"

#include "MainWindow.h"
#include "WeatherWidget.h"

Page::Page(QWidget *parent)
    : QWidget{parent}
    , mainWindow(qobject_cast<MainWindow*>(this->parent()))
{ }

void Page::deleteWidgets()
{
    qDeleteAll(m_widgets);
    m_widgets.clear();
}
