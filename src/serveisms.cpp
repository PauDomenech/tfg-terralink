#include "serveisms.h"
#include "adaptadormodem4g.h"
#include "missatge.h"
#include "contacte.h"
#include <QDebug>

ServeiSMS::ServeiSMS(QObject *parent)
    : IServeiMissatgeria(parent)
{
    m_adaptador = new AdaptadorModem4G(this);
}

bool ServeiSMS::enviar(Missatge *m, Contacte *c)
{
    if (!m || !c) return false;
    qDebug() << "[ServeiSMS] Enviant SMS a:" << c->numeroTelefon;
    return m_adaptador->enviarSMS(c->numeroTelefon, m->text);
}

QList<Missatge*> ServeiSMS::rebre()
{
    return {};
}

QList<Contacte*> ServeiSMS::obtenirContactes()
{
    return {};
}

bool ServeiSMS::estaDisponible()
{
    if (!m_adaptador) return false;
    return m_adaptador->estaDisponibleSMS();
}
