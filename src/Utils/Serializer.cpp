#include "Serializer.h"

#include <QJsonDocument>
#include <QFile>

void Serializer::save(const Serializable &serializable, const QString &filepath)
{
    QJsonDocument doc = QJsonDocument::fromVariant(serializable.toVariant());
    QFile file(filepath);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson(QJsonDocument::JsonFormat::Indented));
    file.close();
}

void Serializer::load(Serializable &serializable, const QString &filepath)
{
    QFile file(filepath);

    if(!file.exists()){
        return;
    }

    file.open(QFile::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    serializable.fromVariant(doc.toVariant());
}

