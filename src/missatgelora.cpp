#include "missatgelora.h"

MissatgeLoRa::MissatgeLoRa(QObject *parent)
    : Missatge(parent)
{
    canal = "lora";
}

QString MissatgeLoRa::serialitzar() const
{
    return QString("LORA|%1|%2|%3").arg(id, nodeDestinacio, text);
}

QString MissatgeLoRa::canal_() const
{
    return "lora";
}
