#include "serveilora.h"
#include "adaptadorlora.h"

ServeiLoRa::ServeiLoRa(QObject *parent)
    : IServeiMissatgeria(parent)
    , m_adaptador(nullptr)
{
    // TODO: inicialitzar adaptador
}

bool ServeiLoRa::enviar(Missatge *m, Contacte *c)
{
    Q_UNUSED(m) Q_UNUSED(c)
    // TODO: cridar m_adaptador per enviar via LoRa
    return false;
}

QList<Missatge*> ServeiLoRa::rebre()
{
    // TODO: obtenir missatges rebuts via adaptador
    return {};
}

QList<Contacte*> ServeiLoRa::obtenirContactes()
{
    return {};
}

bool ServeiLoRa::estaDisponible()
{
    // TODO: comprovar si el mòdul LoRa respon
    return false;
}
