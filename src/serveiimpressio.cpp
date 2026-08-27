#include "serveiimpressio.h"
#include "adaptadorimpressoraescpos.h"
#include "missatge.h"

#include <QDebug>

ServeiImpressio::ServeiImpressio(QObject *parent)
    : QObject(parent)
    , m_ultimCanal("")
{
    m_adaptador = new AdaptadorImpressoraEscPos(this);
}

bool ServeiImpressio::imprimirCapcalera()
{
    return m_adaptador->imprimirCapcalera();
}

bool ServeiImpressio::imprimirMissatge(Missatge *m)
{
    if (!m) return false;
    if (!m_adaptador->estaDisponible()) {
        qDebug() << "[ServeiImpressio] Impressora no disponible";
        return false;
    }

    // Si el canal ha canviat, imprimim la capçalera del nou canal
    bool canviCanal = (m->canal != m_ultimCanal);
    m_ultimCanal = m->canal;

    m_adaptador->imprimirMissatgeFormat(
        m->canal,
        m->dataCreacio,
        m->text,
        canviCanal
        );

    // Imprimir foto si n'hi ha
    if (!m->rutaAudio.isEmpty()) {
        m_adaptador->imprimirImatgeDesDeRuta(m->rutaAudio);
    }

    return true;
}
