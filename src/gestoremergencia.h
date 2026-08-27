#ifndef GESTOREMERGENCIA_H
#define GESTOREMERGENCIA_H

#include <QObject>
#include <QTimer>
#include <QList>

class IServeiMissatgeria;
class Missatge;
class Contacte;
class ServeiMonitoratge;

/**
 * @brief Gestiona el protocol d'emergència. Patró Strategy (broadcast multicanal).
 *
 * Envia missatges d'emergència simultàniament per tots els canals disponibles
 * (WhatsApp, SMS, LoRa). Emet signals per notificar l'estat de cada enviament
 * a la UI, permetent mostrar feedback visual per canal.
 *
 * @see ControladorAplicacio, IServeiMissatgeria, GestorCanals
 */
class GestorEmergencia : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit GestorEmergencia(QObject *parent = nullptr);

    ServeiMonitoratge          *monitor;           ///< Monitor de l'estat del sistema
    QList<IServeiMissatgeria*>  canalsOrdenats;    ///< Canals per ordre de prioritat (WA > SMS > LoRa)
    int                         timeoutMs = 10000; ///< Timeout màxim per canal en ms

    /**
     * @brief Envia el missatge d'emergència per tots els canals disponibles (broadcast).
     * Per cada canal: comprova disponibilitat, envia i emet canalEnviatOk o canalFallit.
     * @param m Missatge d'emergència a enviar (text "SOS - EMERGÈNCIA").
     * @param c Contacte destinatari.
     */
    void enviarEmergencia(Missatge *m, Contacte *c);

    /**
     * @brief Selecciona el primer canal disponible de la llista ordenada.
     * @return El primer canal disponible, o nullptr si cap canal respon.
     */
    IServeiMissatgeria* seleccionarMillorCanal();

signals:
    /**
     * @brief S'emet quan un canal ha enviat l'emergència correctament.
     * @param canal Punter al servei que ha enviat.
     */
    void canalEnviatOk(IServeiMissatgeria *canal);

    /**
     * @brief S'emet quan un canal no estava disponible o ha fallat l'enviament.
     * @param canal Punter al servei que ha fallat.
     */
    void canalFallit(IServeiMissatgeria *canal);

    /**
     * @brief S'emet quan s'han processat tots els canals.
     * @param ok true si almenys un canal ha enviat correctament.
     * @param fallits Nombre de canals que han fallat.
     */
    void totsCanalsCompletats(bool ok, int fallits);

private slots:
    /** @brief Gestiona el timeout d'un canal (marcat com a fallat). */
    void onTimeout();

private:
    QList<QTimer*> m_timers; ///< Timers de timeout per a cada canal actiu
};

#endif // GESTOREMERGENCIA_H
