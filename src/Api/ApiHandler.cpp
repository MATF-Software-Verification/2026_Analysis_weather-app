#include "ApiHandler.h"

ApiHandler::ApiHandler(QObject *parent)
    : QObject{parent}
    , networkManager(new QNetworkAccessManager())
{
    networkManager->setTransferTimeout(2000);
}

ApiHandler::~ApiHandler()
{
    delete networkManager;
}
