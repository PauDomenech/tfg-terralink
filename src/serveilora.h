#ifndef SERVEILORA_H
#define SERVEILORA_H

#include "iserveimissatgeria.h"

class AdaptadorLoRa;

/**
 * @brief Implementació del servei de missatgeria via LoRa (Meshtastic). Patró Strategy.
 *
 * Implementa IServeiMissatgeria usant l'AdaptadorLoRa per enviar
 * missatges per ràdio LoRa quan no hi ha cobertura mòbil ni WiFi.
 * Radi d'acció típic: 1-15 km en zona oberta.
 *
 * @note Implementació pendent (TODO): tots els mètodes retornen false/buit.
 * @see IServeiMissatgeria, AdaptadorLoRa, GestorCanals
 */
class ServeiLoRa : public IServeiMissatgeria
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit ServeiLoRa(QObject *parent = nullptr);

    /**
     * @brief Envia un missatge via LoRa al node del contacte.
     * @param m Missatge a enviar (text truncat a MissatgeLoRa::MAX_PAYLOAD_BYTES).
     * @param c Contacte destinatari (s'usa c->adrecaLora com a node destí).
     * @return true si l'enviament ha estat correcte (TODO: sempre false).
     */
    bool             enviar(Missatge *m, Contacte *c) override;

    /**
     * @brief Obté els missatges LoRa rebuts.
     * @return Llista de missatges rebuts (TODO: sempre buida).
     */
    QList<Missatge*> rebre() override;

    /**
     * @brief Obté els nodes LoRa visibles a la xarxa Meshtastic.
     * @return Llista de contactes disponibles (TODO: sempre buida).
     */
    QList<Contacte*> obtenirContactes() override;

    /**
     * @brief Comprova si el mòdul LoRa respon.
     * @return true si el mòdul LoRa és accessible (TODO: sempre false).
     */
    bool             estaDisponible() override;

private:
    AdaptadorLoRa *m_adaptador; ///< Adaptador del mòdul LoRa (SPI/UART)
};

#endif // SERVEILORA_H
