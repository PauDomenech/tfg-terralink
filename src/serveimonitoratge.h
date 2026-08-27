#ifndef SERVEIMONITORATGE_H
#define SERVEIMONITORATGE_H

#include <QObject>
#include <QTimer>

class EstatSistema;

/**
 * @brief Servei de monitoratge de l'estat del sistema. Patró Singleton + Observer.
 *
 * Comprova periòdicament l'estat dels diferents components
 * (WhatsApp, SMS, LoRa) i notifica els canvis via el signal
 * estatCanviat (patró Observer implementat amb Qt signals/slots).
 * La comprovació es fa cada 5 segons mitjançant un QTimer intern,
 * engegat via iniciarMonitoratge().
 *
 * @see EstatSistema, GestorCanals, GestorEmergencia
 */
class ServeiMonitoratge : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Retorna la instància única del servei (Singleton).
     * @return Referència a la instància única.
     */
    static ServeiMonitoratge& instancia();

    /**
     * @brief Retorna l'estat actual del sistema.
     * @return Punter a l'objecte EstatSistema amb els flags de disponibilitat.
     */
    EstatSistema* obtenerEstatSistema();

    /**
     * @brief Engega el monitoratge periòdic (cada 5 segons).
     *
     * Fa una primera comprovació immediata i, a partir d'aquí, en repeteix
     * l'execució via QTimer. Cal cridar-lo un cop, típicament des de
     * ControladorAplicacio::inicialitzar().
     */
    void iniciarMonitoratge();

public slots:
    /**
     * @brief Comprova l'estat de tots els canals i emet estatCanviat.
     *
     * Consulta IServeiMissatgeria::estaDisponible() de cada canal
     * (WhatsApp, SMS, LoRa) via GestorCanals, actualitza els flags
     * de l'EstatSistema intern i emet el signal estatCanviat.
     * Cridat periòdicament pel QTimer intern, o manualment si cal
     * una actualització immediata.
     */
    void notificarCanviEstat();

signals:
    /**
     * @brief S'emet quan l'estat d'algun component del sistema canvia.
     * @param estat Punter a l'EstatSistema actualitzat.
     */
    void estatCanviat(EstatSistema *estat);

private:
    ServeiMonitoratge(QObject *parent = nullptr);
    ServeiMonitoratge(const ServeiMonitoratge&) = delete;
    ServeiMonitoratge& operator=(const ServeiMonitoratge&) = delete;

    EstatSistema *m_estat;        ///< Estat actual de tots els components del sistema
    QTimer       *m_timer = nullptr; ///< Timer de polling de l'estat cada 5 segons
};

#endif // SERVEIMONITORATGE_H
