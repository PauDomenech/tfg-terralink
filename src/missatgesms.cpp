#include "missatgesms.h"

MissatgeSMS::MissatgeSMS(QObject *parent)
    : Missatge(parent)
{
    canal = "sms";
}

QString MissatgeSMS::serialitzar() const
{
    return QString("SMS|%1|%2|%3").arg(id, numeroTelefon, text);
}

QString MissatgeSMS::canal_() const
{
    return "sms";
}
