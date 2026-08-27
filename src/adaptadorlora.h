#ifndef ADAPTADORLORA_H
#define ADAPTADORLORA_H

#include <QObject>
#include <QString>

/**
 * @brief Adaptador per al mòdul LoRa (Meshtastic).
 *
 * Gestiona la comunicació amb el mòdul LoRa via UART/SPI
 * per enviar missatges de ràdio quan no hi ha cobertura mòbil.
 *
 * @see ServeiLoRa
 */
class AdaptadorLoRa : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit AdaptadorLoRa(QObject *parent = nullptr);

    /**
     * @brief Envia un paquet de dades via LoRa.
     * @param nodeDestinacio Identificador del node destinatari.
     * @param payload Dades a enviar (màx 240 bytes).
     * @return true si s'ha enviat correctament.
     */
    bool enviar(const QString &nodeDestinacio, const QString &payload);

    /** @brief Comprova si el mòdul LoRa respon. @return true si disponible. */
    bool estaDisponible();

private:
    QString m_port; ///< Port UART del mòdul LoRa (ex: "/dev/ttyAMA0")
};

#endif // ADAPTADORLORA_H
