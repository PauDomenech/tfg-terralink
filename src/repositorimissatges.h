#ifndef REPOSITORIMISSATGES_H
#define REPOSITORIMISSATGES_H

#include <QObject>
#include <QList>

class Missatge;

/**
 * @brief Repositori de missatges en memòria. Patró Singleton.
 *
 * Gestiona l'emmagatzematge i recuperació dels missatges enviats i rebuts
 * durant la sessió. Els missatges no es persiteixen a disc (es perden en
 * tancar l'aplicació).
 *
 * @see Missatge, ControladorAplicacio, MissatgesRebuts
 */
class RepositoriMissatges : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Retorna la instància única del repositori (Singleton).
     * @return Referència a la instància única.
     */
    static RepositoriMissatges& instancia();

    /**
     * @brief Afegeix un missatge al repositori.
     * @param m Missatge a desar (no pot ser nullptr).
     * @return true si s'ha desat correctament.
     */
    bool desar(Missatge *m);

    /**
     * @brief Retorna els missatges més recents de la sessió.
     * @return Llista dels últims 20 missatges (enviats i rebuts).
     */
    QList<Missatge*> trobarRecents();

private:
    RepositoriMissatges(QObject *parent = nullptr);
    RepositoriMissatges(const RepositoriMissatges&) = delete;
    RepositoriMissatges& operator=(const RepositoriMissatges&) = delete;

    QList<Missatge*> m_missatges; ///< Llista interna de tots els missatges de la sessió
};

#endif // REPOSITORIMISSATGES_H
