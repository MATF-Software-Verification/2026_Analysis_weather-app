#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Serializable.h"

class Serializer
{
public:
    Serializer() = default;
    ~Serializer() = default;
    static void save(const Serializable& serializable, const QString& filepath);
    static void load(Serializable& serializable, const QString& filepath);
};

#endif // SERIALIZER_H
