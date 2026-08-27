#ifndef REPOSITORICONTACTES_H
#define REPOSITORICONTACTES_H

#include <QObject>
#include <QList>

class Contacte;

/**
 * @brief Repositori de contactes. Patró Singleton.
 *
 * Gestiona el cicle de vida dels contactes del sistema.
 * Proporciona operacions CRUD, persistència en JSON i filtratge
 * per contactes d'emergència.
 *
 * La persistència usa /home/paudomenech/TerraLink/contactes.json
 * i es carrega automàticament a l'inici (constructor privat).
 *
 * @see Contacte, ControladorAplicacio
 */
class RepositoriContactes : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Retorna la instància única del repositori (Singleton).
     * @return Referència a la instància única.
     */
    static RepositoriContactes& instancia();

    /**
     * @brief Retorna tots els contactes desats.
     * @return Llista de punters a Contacte.
     */
    QList<Contacte*> trobarTots();

    /**
     * @brief Cerca un contacte per identificador UUID.
     * @param id Identificador UUID del contacte.
     * @return Punter al contacte, o nullptr si no existeix.
     */
    Contacte* trobarPerId(const QString &id);

    /**
     * @brief Desa o actualitza un contacte al repositori i al fitxer JSON.
     * Si ja existeix un contacte amb el mateix número, actualitza el nom.
     * @param c Contacte a desar.
     * @return true si s'ha desat correctament.
     */
    bool desar(Contacte *c);

    /**
     * @brief Elimina un contacte per identificador i actualitza el fitxer JSON.
     * @param id Identificador UUID del contacte a eliminar.
     * @return true si s'ha eliminat correctament.
     */
    bool eliminar(const QString &id);

    /** @brief Recarrega els contactes des del fitxer JSON (reservat per ús futur). */
    void recarregar();

    /**
     * @brief Retorna els contactes marcats com a emergència.
     * @return Llista de contactes amb esEmergencia = true.
     */
    QList<Contacte*> trobarEmergencies();

private:
    RepositoriContactes(QObject *parent = nullptr);
    RepositoriContactes(const RepositoriContactes&) = delete;
    RepositoriContactes& operator=(const RepositoriContactes&) = delete;

    QList<Contacte*> m_contactes; ///< Llista interna de contactes carregats

    QString m_rutaFitxer = "/home/paudomenech/TerraLink/contactes.json"; ///< Ruta de persistència JSON

    /** @brief Serialitza tots els contactes a JSON i els escriu al fitxer. */
    void guardarAFitxer();

    /** @brief Carrega els contactes del fitxer JSON a m_contactes. */
    void carregarDeFitxer();
};

#endif // REPOSITORICONTACTES_H
