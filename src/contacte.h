#ifndef CONTACTE_H
#define CONTACTE_H

#include <QObject>

/**
 * @brief Representa un contacte del sistema.
 *
 * Entitat de dades que conté la informació d'un destinatari:
 * nom, número de telèfon, adreça LoRa i si és contacte d'emergència.
 *
 * @see RepositoriContactes
 */
class Contacte : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit Contacte(QObject *parent = nullptr);

    QString id;             ///< Identificador únic (UUID)
    QString nom;            ///< Nom del contacte
    QString numeroTelefon;  ///< Número de telèfon (amb prefix de país, ex: 34659...)
    QString adrecaLora;     ///< Adreça del node LoRa (Meshtastic)
    bool    esEmergencia = false; ///< true si ha de rebre missatges d'emergència
};

#endif // CONTACTE_H
