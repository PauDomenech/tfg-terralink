#ifndef SERVEIWHATSAPP_H
#define SERVEIWHATSAPP_H

#include "iserveimissatgeria.h"

class AdaptadorWhatsApp;

/**
 * @brief Implementació del servei de missatgeria via WhatsApp. Patró Strategy.
 *
 * Implementa IServeiMissatgeria usant l'AdaptadorWhatsApp per comunicar-se
 * amb el procés Node.js (whatsapp-web.js) via stdin/stdout.
 * La recepció de missatges arriba via el signal missatgeRebut de l'adaptador.
 *
 * @see IServeiMissatgeria, AdaptadorWhatsApp, GestorCanals
 */
class ServeiWhatsApp : public IServeiMissatgeria
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit ServeiWhatsApp(QObject *parent = nullptr);

    /** @brief Inicia el procés Node.js de WhatsApp (whatsapp.js). */
    void iniciarConnexio();

    /** @brief Atura el procés Node.js de WhatsApp de forma segura. */
    void aturarConnexio();

    /**
     * @brief Envia un missatge WhatsApp al contacte.
     * @param m Missatge a enviar (s'usa m->text).
     * @param c Contacte destinatari (s'usa c->numeroTelefon).
     * @return true si Node ha confirmat l'enviament (SEND_OK).
     */
    bool enviar(Missatge *m, Contacte *c) override;

    /**
     * @brief Obté els missatges rebuts (no usat; la recepció és via signal missatgeRebut).
     * @return Llista buida.
     */
    QList<Missatge*> rebre() override;

    /**
     * @brief Demana la llista de contactes al Node i espera la resposta (màx 10s).
     * @return Contactes del RepositoriContactes (actualitzats després de la importació).
     */
    QList<Contacte*> obtenirContactes() override;

    /**
     * @brief Comprova si WhatsApp està connectat i llest per enviar.
     * @return true si el procés Node ha arribat a APP_READY.
     */
    bool estaDisponible() override;

    /**
     * @brief Retorna l'adaptador intern per connectar els seus signals (QR, connectat, etc.).
     * @return Punter a l'AdaptadorWhatsApp.
     */
    AdaptadorWhatsApp* adaptador() const { return m_adaptador; }

private:
    AdaptadorWhatsApp *m_adaptador; ///< Adaptador que gestiona el procés Node.js
};

#endif // SERVEIWHATSAPP_H
