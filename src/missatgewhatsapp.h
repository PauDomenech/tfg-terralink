#ifndef MISSATGEWHATSAPP_H
#define MISSATGEWHATSAPP_H

#include "missatge.h"

/**
 * @brief Missatge enviat per WhatsApp.
 *
 * Subclasse concreta de Missatge per al canal WhatsApp.
 * Creada per MissatgeFactory amb canal "whatsapp".
 *
 * @see Missatge, MissatgeFactory, ServeiWhatsApp
 */
class MissatgeWhatsApp : public Missatge
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit MissatgeWhatsApp(QObject *parent = nullptr);

    QString numeroWA; ///< Número de WhatsApp destinatari (amb prefix país)

    /** @brief Serialitza el missatge en format "WA|id|numero|text". */
    QString serialitzar() const override;

    /** @brief Retorna "whatsapp". */
    QString canal_() const override;
};

#endif // MISSATGEWHATSAPP_H
