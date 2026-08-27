#include "controladoraplicacio.h"
#include "gestoremergencia.h"
#include "gestorcanals.h"
#include "missatgefactory.h"
#include "repositoricontactes.h"
#include "repositorimissatges.h"
#include "iserveimissatgeria.h"
#include "serveimonitoratge.h"
#include "serveiaudio.h"
#include "serveiwhatsapp.h"
#include "serveiimpressio.h"
#include "serveisms.h"
#include "adaptadorwhatsapp.h"
#include "adaptadormodem4g.h"
#include "missatge.h"
#include "missatgewhatsapp.h"
#include "missatgesms.h"
#include "contacte.h"

ControladorAplicacio::ControladorAplicacio(QObject *parent)
    : QObject(parent)
{
    gestorEmergencia  = new GestorEmergencia(this);
    m_serveiAudio     = new ServeiAudio(this);
    m_serveiImpressio = new ServeiImpressio(this);
}

ControladorAplicacio& ControladorAplicacio::instancia()
{
    static ControladorAplicacio inst;
    return inst;
}

void ControladorAplicacio::inicialitzar()
{
    // Connectar WhatsApp i escoltar missatges entrants via AdaptadorWhatsApp
    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (sw) {
        sw->iniciarConnexio();
        connect(sw->adaptador(), &AdaptadorWhatsApp::missatgeRebut,
                this, &ControladorAplicacio::onMissatgeWhatsAppRebut);
    }

    // Inicialitzar canals per ordre de prioritat al GestorEmergencia
    gestorEmergencia->canalsOrdenats = {
        GestorCanals::obtenir("whatsapp"),
        GestorCanals::obtenir("sms"),
        GestorCanals::obtenir("lora")
    };
    gestorEmergencia->monitor = &ServeiMonitoratge::instancia();

    // Capçalera d'inici a la impressora
    m_serveiImpressio->imprimirCapcalera();

    // Iniciar polling SMS via AdaptadorModem4G
    ServeiSMS *ss = qobject_cast<ServeiSMS*>(GestorCanals::obtenir("sms"));
    if (ss) {
        connect(ss->adaptador(), &AdaptadorModem4G::smsRebut,
                this, &ControladorAplicacio::onSMSRebut);
        ss->adaptador()->iniciarPollingSMS();
    }
}

void ControladorAplicacio::enviarMissatgeText(const QString &text)
{
    QList<Contacte*> contactes = RepositoriContactes::instancia().trobarTots();
    if (contactes.isEmpty()) return;

    Contacte *destinatari = contactes.first();
    QList<Missatge*> missatges = MissatgeFactory::crearTots(text, destinatari);

    for (Missatge *m : missatges) {
        IServeiMissatgeria *canal = GestorCanals::obtenir(m->canal);
        if (canal && canal->estaDisponible()) {
            canal->enviar(m, destinatari) ? m->marcarEnviat() : m->marcarFallit();
        } else {
            m->marcarFallit();
        }
        RepositoriMissatges::instancia().desar(m);
    }
}

void ControladorAplicacio::transcriureAudio()
{
    QString text = m_serveiAudio->aturarGravacio();
    if (!text.isEmpty())
        enviarMissatgeText(text);
}

void ControladorAplicacio::processarMissatgeEntrant(Missatge *m)
{
    if (!m) return;
    RepositoriMissatges::instancia().desar(m);
    m_serveiImpressio->imprimirMissatge(m);
}

void ControladorAplicacio::activarEmergencia(Contacte *c)
{
    Q_UNUSED(c)
    QList<Contacte*> contactes = RepositoriContactes::instancia().trobarEmergencies();

    if (contactes.isEmpty()) {
        qDebug() << "[Emergencia] No hi ha contactes d'emergència configurats!";
        return;
    }

    for (Contacte *destinatari : contactes) {
        Missatge *m = MissatgeFactory::crear("whatsapp", "SOS - EMERGÈNCIA", destinatari);
        gestorEmergencia->enviarEmergencia(m, destinatari);
    }
}

void ControladorAplicacio::onMissatgeWhatsAppRebut(const QString &contingut)
{
    MissatgeWhatsApp *m = new MissatgeWhatsApp();
    m->canal = "whatsapp";
    m->marcarRebut();

    if (contingut.contains("[FOTO]")) {
        int idx  = contingut.indexOf("[FOTO]");
        m->text      = contingut.left(idx).trimmed();
        m->rutaAudio = contingut.mid(idx + 6).trimmed();
    } else {
        m->text = contingut;
    }

    processarMissatgeEntrant(m);
}

bool ControladorAplicacio::iniciarGravacio()
{
    return m_serveiAudio->iniciarGravacio();
}

void ControladorAplicacio::transcriureIEnviaA(Contacte *c)
{
    if (!c) return;
    QString text = m_serveiAudio->aturarGravacio();
    if (text.isEmpty()) return;

    Missatge *m = MissatgeFactory::crear("whatsapp", text, c);
    IServeiMissatgeria *canal = GestorCanals::obtenir("whatsapp");
    if (canal && canal->estaDisponible())
        canal->enviar(m, c) ? m->marcarEnviat() : m->marcarFallit();
    else
        m->marcarFallit();

    RepositoriMissatges::instancia().desar(m);
}

QString ControladorAplicacio::aturarITranscriure()
{
    return m_serveiAudio->aturarGravacio();
}

void ControladorAplicacio::enviarTextAContacte(const QString &text, Contacte *c)
{
    if (!c || text.isEmpty()) return;

    Missatge *m = MissatgeFactory::crear("whatsapp", text, c);
    IServeiMissatgeria *canal = GestorCanals::obtenir("whatsapp");
    if (canal && canal->estaDisponible())
        canal->enviar(m, c) ? m->marcarEnviat() : m->marcarFallit();
    else
        m->marcarFallit();

    RepositoriMissatges::instancia().desar(m);
}

void ControladorAplicacio::onSMSRebut(const QString &telefon, const QString &text, const QString &data)
{
    Q_UNUSED(data)

    // Intentar substituir el número pel nom del contacte
    QString nomMostrar = telefon;
    QString numeroNet  = QString(telefon).remove('+');

    for (Contacte *c : RepositoriContactes::instancia().trobarTots()) {
        if (c->numeroTelefon == numeroNet || c->numeroTelefon == telefon) {
            nomMostrar = c->nom;
            break;
        }
    }

    qDebug() << "[Controlador] SMS rebut de:" << nomMostrar;

    MissatgeSMS *m = new MissatgeSMS();
    m->text  = nomMostrar + ": " + text;
    m->canal = "sms";
    m->marcarRebut();

    processarMissatgeEntrant(m);
}
