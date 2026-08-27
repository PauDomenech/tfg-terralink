#ifndef ISERVEIMISSATGERIA_H
#define ISERVEIMISSATGERIA_H

#include <QObject>
#include <QList>

class Missatge;
class Contacte;

/**
 * @brief Interfície per a tots els serveis de missatgeria.
 *
 * Defineix el contracte que han de complir totes les implementacions
 * de canal de comunicació (WhatsApp, SMS, LoRa).
 * Aplica el patró Strategy.
 *
 * @see ServeiWhatsApp, ServeiSMS, ServeiLoRa, GestorCanals, GestorEmergencia
 */
class IServeiMissatgeria : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit IServeiMissatgeria(QObject *parent = nullptr);

    /**
     * @brief Envia un missatge a un contacte.
     * @param m Missatge a enviar.
     * @param c Contacte destinatari.
     * @return true si l'enviament ha estat correcte.
     */
    virtual bool enviar(Missatge *m, Contacte *c) = 0;

    /**
     * @brief Obté els missatges rebuts.
     * @return Llista de missatges rebuts.
     */
    virtual QList<Missatge*> rebre() = 0;

    /**
     * @brief Obté la llista de contactes del canal.
     * @return Llista de contactes disponibles.
     */
    virtual QList<Contacte*> obtenirContactes() = 0;

    /**
     * @brief Comprova si el canal està disponible.
     * @return true si el canal pot enviar missatges.
     */
    virtual bool estaDisponible() = 0;

    virtual ~IServeiMissatgeria() = default;
};

#endif // ISERVEIMISSATGERIA_H
