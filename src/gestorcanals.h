#ifndef GESTORCANALS_H
#define GESTORCANALS_H

#include <QMap>
#include <QString>

class IServeiMissatgeria;

/**
 * @brief Gestiona les instàncies dels serveis de missatgeria. Patró Multiton.
 *
 * Garanteix que existeixi una sola instància per canal (whatsapp, sms, lora).
 * Les crea sota demanda (lazy initialization) i les reutilitza en cridades posteriors.
 * Tots els mètodes són estàtics: no cal instanciar GestorCanals.
 *
 * @see IServeiMissatgeria, ServeiWhatsApp, ServeiSMS, ServeiLoRa
 */
class GestorCanals
{
public:
    /**
     * @brief Obté la instància del servei per al canal indicat.
     * Si no existeix, la crea (lazy initialization).
     * @param canal Nom del canal: "whatsapp", "sms" o "lora".
     * @return Punter al servei, o nullptr si el canal no és vàlid.
     */
    static IServeiMissatgeria* obtenir(const QString &canal);

    /**
     * @brief Retorna els canals que responen com a disponibles en aquest moment.
     * @return Llista de noms de canals disponibles (ex: ["whatsapp"]).
     */
    static QStringList canalsDisponibles();

    /** @brief Allibera la memòria de totes les instàncies i neteja el mapa intern. */
    static void alliberar();

private:
    /**
     * @brief Retorna la referència al mapa intern de serveis (static local).
     * @return Mapa canal → punter a IServeiMissatgeria.
     */
    static QMap<QString, IServeiMissatgeria*>& instancies();
};

#endif // GESTORCANALS_H
