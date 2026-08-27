#ifndef ESTATSISTEMA_H
#define ESTATSISTEMA_H

#include <QObject>

/**
 * @brief Representa l'estat actual de tots els components del sistema.
 *
 * Entitat de dades que conté flags de disponibilitat per a cada
 * component de comunicació i perifèric.
 *
 * @see ServeiMonitoratge
 */
class EstatSistema : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit EstatSistema(QObject *parent = nullptr);

    bool internetDisponible  = false; ///< Connexió a internet disponible
    bool whatsappDisponible  = false; ///< WhatsApp (whatsapp-web.js) connectat i llest
    bool smsDisponible       = false; ///< Mòdem 4G per SMS disponible
    bool loraDisponible      = false; ///< Mòdul LoRa disponible
    bool impressoraPreparada = false; ///< Impressora ESC/POS disponible
    bool audioPreparada      = false; ///< Micròfon/altaveu disponible
};

#endif // ESTATSISTEMA_H
