#ifndef MISSATGESMS_H
#define MISSATGESMS_H

#include "missatge.h"

/**
 * @brief Missatge enviat per SMS.
 *
 * Subclasse concreta de Missatge per al canal SMS via mòdem 4G.
 * Creada per MissatgeFactory amb canal "sms".
 *
 * @see Missatge, MissatgeFactory, ServeiSMS
 */
class MissatgeSMS : public Missatge
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit MissatgeSMS(QObject *parent = nullptr);

    QString numeroTelefon; ///< Número de telèfon destinatari

    /** @brief Serialitza el missatge en format "SMS|id|numero|text". */
    QString serialitzar() const override;

    /** @brief Retorna "sms". */
    QString canal_() const override;
};

#endif // MISSATGESMS_H
