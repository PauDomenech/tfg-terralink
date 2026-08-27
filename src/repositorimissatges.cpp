#include "repositorimissatges.h"

RepositoriMissatges::RepositoriMissatges(QObject *parent)
    : QObject(parent)
{
    // TODO: carregar des de fitxer/BD
}

RepositoriMissatges& RepositoriMissatges::instancia()
{
    static RepositoriMissatges inst;
    return inst;
}

bool RepositoriMissatges::desar(Missatge *m)
{
    if (!m) return false;
    m_missatges.append(m);
    return true;
}

QList<Missatge*> RepositoriMissatges::trobarRecents()
{
    // Retorna els últims 20
    if (m_missatges.size() <= 20)
        return m_missatges;

    return m_missatges.mid(m_missatges.size() - 20);
}
