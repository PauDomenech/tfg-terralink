#ifndef MISSATGEFACTORY_H
#define MISSATGEFACTORY_H

#include <QList>
#include <QString>

class Contacte;
class Missatge;

/**
 * @brief Factoria per crear instàncies de Missatge. Patró Factory.
 *
 * Crea el tipus correcte de Missatge (WhatsApp, SMS o LoRa)
 * segons el canal especificat. Totes les operacions són estàtiques.
 *
 * @see Missatge, MissatgeWhatsApp, MissatgeSMS, MissatgeLoRa
 */
class MissatgeFactory
{
public:
    /**
     * @brief Crea un missatge per a un canal específic.
     * @param canal Canal destí: "whatsapp", "sms" o "lora".
     * @param text Text del missatge.
     * @param c Contacte destinatari.
     * @return Missatge creat, o nullptr si el canal no és vàlid.
     */
    static Missatge* crear(const QString &canal, const QString &text, Contacte *c);

    /**
     * @brief Crea un missatge per a cada canal disponible.
     * @param text Text del missatge.
     * @param c Contacte destinatari.
     * @return Llista amb un missatge per cada canal (WhatsApp, SMS, LoRa).
     */
    static QList<Missatge*> crearTots(const QString &text, Contacte *c);
};

#endif // MISSATGEFACTORY_H
