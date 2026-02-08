#include "ErrorPage.h"

#include "MainWindow.h"

ErrorPage::ErrorPage(QWidget *parent)
    : Page{parent}
    , mainLayout(new QVBoxLayout(this))
    , upperLayout(new QHBoxLayout())
    , errorLabel(new QLabel(this))
    , homeButton(new QPushButton("< Home", this))
{
    mainLayout->addLayout(upperLayout);
    upperLayout->addWidget(homeButton);
    upperLayout->addStretch(1);
    mainLayout->addWidget(errorLabel, 0, Qt::AlignCenter);

    connect(homeButton, &QPushButton::clicked, mainWindow, &MainWindow::showHomePage);
}

void ErrorPage::setErrMsg(const QString &errMsg)
{
    errorLabel->setText(errMsg);
}
