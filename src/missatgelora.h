#ifndef MISSATGELORA_H
#define MISSATGELORA_H

#include "missatge.h"

/**
 * @brief Missatge enviat per LoRa (Meshtastic).
 *
 * Subclasse concreta de Missatge per al canal LoRa.
 * Limitat a MAX_PAYLOAD_BYTES caràcters per les restriccions del protocol.
 * Creada per MissatgeFactory amb canal "lora".
 *
 * @see Missatge, MissatgeFactory, ServeiLoRa
 */
class MissatgeLoRa : public Missatge
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit MissatgeLoRa(QObject *parent = nullptr);

    QString nodeDestinacio;              ///< Identificador del node LoRa destinatari
    static const int MAX_PAYLOAD_BYTES = 240; ///< Mida màxima del payload LoRa

    /** @brief Serialitza el missatge en format "LORA|id|node|text". */
    QString serialitzar() const override;

    /** @brief Retorna "lora". */
    QString canal_() const override;
};

#endif // MISSATGELORA_H
