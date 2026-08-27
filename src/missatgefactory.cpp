#include "missatgefactory.h"
#include "missatgewhatsapp.h"
#include "missatgesms.h"
#include "missatgelora.h"
#include "contacte.h"

Missatge* MissatgeFactory::crear(const QString &canal, const QString &text, Contacte *c)
{
    Q_UNUSED(c)

    if (canal == "whatsapp") {
        MissatgeWhatsApp *m = new MissatgeWhatsApp();
        m->text = text;
        return m;
    }
    else if (canal == "sms") {
        MissatgeSMS *m = new MissatgeSMS();
        m->text = text;
        return m;
    }
    else if (canal == "lora") {
        MissatgeLoRa *m = new MissatgeLoRa();
        m->text = text;
        return m;
    }

    return nullptr;
}

QList<Missatge*> MissatgeFactory::crearTots(const QString &text, Contacte *c)
{
    QList<Missatge*> llista;
    llista.append(crear("whatsapp", text, c));
    llista.append(crear("sms",      text, c));
    llista.append(crear("lora",     text, c));
    return llista;
}
