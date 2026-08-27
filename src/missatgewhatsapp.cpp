#include "missatgewhatsapp.h"

MissatgeWhatsApp::MissatgeWhatsApp(QObject *parent)
    : Missatge(parent)
{
    canal = "whatsapp";
}

QString MissatgeWhatsApp::serialitzar() const
{
    return QString("WA|%1|%2|%3").arg(id, numeroWA, text);
}

QString MissatgeWhatsApp::canal_() const
{
    return "whatsapp";
}
