#include "serveiwhatsapp.h"
#include "adaptadorwhatsapp.h"
#include "contacte.h"
#include "missatge.h"
#include "repositoricontactes.h"

#include <QEventLoop>
#include <QTimer>

ServeiWhatsApp::ServeiWhatsApp(QObject *parent)
    : IServeiMissatgeria(parent)
{
    m_adaptador = new AdaptadorWhatsApp(this);
}

void ServeiWhatsApp::iniciarConnexio()
{
    m_adaptador->iniciarProces();
}

void ServeiWhatsApp::aturarConnexio()
{
    m_adaptador->aturarProces();
}

bool ServeiWhatsApp::enviar(Missatge *m, Contacte *c)
{
    if (!m || !c) return false;
    return m_adaptador->enviarWhatsApp(c->numeroTelefon, m->text);
}

QList<Missatge*> ServeiWhatsApp::rebre()
{
    return {};
}

QList<Contacte*> ServeiWhatsApp::obtenirContactes()
{
    if (!m_adaptador->estaDisponible())
        return RepositoriContactes::instancia().trobarTots();

    m_adaptador->demanarContactes();

    // Esperem la resposta del Node (màx 10s)
    QEventLoop bucle;
    QTimer temporitzador;
    temporitzador.setSingleShot(true);
    temporitzador.setInterval(10000);

    connect(m_adaptador, &AdaptadorWhatsApp::contactesRebuts, &bucle, &QEventLoop::quit);
    connect(&temporitzador, &QTimer::timeout, &bucle, &QEventLoop::quit);

    temporitzador.start();
    bucle.exec();

    return RepositoriContactes::instancia().trobarTots();
}

bool ServeiWhatsApp::estaDisponible()
{
    return m_adaptador->estaDisponible();
}
