#include "serveimonitoratge.h"
#include "estatsistema.h"
#include "gestorcanals.h"
#include "iserveimissatgeria.h"

ServeiMonitoratge::ServeiMonitoratge(QObject *parent)
    : QObject(parent)
{
    m_estat = new EstatSistema();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ServeiMonitoratge::notificarCanviEstat);
}

ServeiMonitoratge& ServeiMonitoratge::instancia()
{
    static ServeiMonitoratge inst;
    return inst;
}

EstatSistema* ServeiMonitoratge::obtenerEstatSistema()
{
    return m_estat;
}

void ServeiMonitoratge::iniciarMonitoratge()
{
    notificarCanviEstat(); // primera comprovació immediata
    m_timer->start(5000);  // cada 5 segons
}

void ServeiMonitoratge::notificarCanviEstat()
{
    IServeiMissatgeria *wa   = GestorCanals::obtenir("whatsapp");
    IServeiMissatgeria *sms  = GestorCanals::obtenir("sms");
    IServeiMissatgeria *lora = GestorCanals::obtenir("lora");

    m_estat->whatsappDisponible = wa   && wa->estaDisponible();
    m_estat->smsDisponible      = sms  && sms->estaDisponible();
    m_estat->loraDisponible     = lora && lora->estaDisponible();

    emit estatCanviat(m_estat);
}
