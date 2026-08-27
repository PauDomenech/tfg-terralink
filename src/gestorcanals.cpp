#include "gestorcanals.h"
#include "iserveimissatgeria.h"
#include "serveiwhatsapp.h"
#include "serveisms.h"
#include "serveilora.h"

QMap<QString, IServeiMissatgeria*>& GestorCanals::instancies()
{
    static QMap<QString, IServeiMissatgeria*> mapa;
    return mapa;
}

IServeiMissatgeria* GestorCanals::obtenir(const QString &canal)
{
    auto &mapa = instancies();

    // Si no existeix encara, el creem (Multiton)
    if (!mapa.contains(canal)) {
        if (canal == "whatsapp")
            mapa[canal] = new ServeiWhatsApp();
        else if (canal == "sms")
            mapa[canal] = new ServeiSMS();
        else if (canal == "lora")
            mapa[canal] = new ServeiLoRa();
        else
            return nullptr;
    }

    return mapa[canal];
}

QStringList GestorCanals::canalsDisponibles()
{
    QStringList disponibles;
    for (auto it = instancies().begin(); it != instancies().end(); ++it) {
        if (it.value()->estaDisponible())
            disponibles.append(it.key());
    }
    return disponibles;
}

void GestorCanals::alliberar()
{
    for (IServeiMissatgeria *s : instancies())
        delete s;
    instancies().clear();
}
