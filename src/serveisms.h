#ifndef SERVEISMS_H
#define SERVEISMS_H

#include "iserveimissatgeria.h"

class AdaptadorModem4G;

/**
 * @brief Implementació del servei de missatgeria via SMS. Patró Strategy.
 *
 * Implementa IServeiMissatgeria usant l'AdaptadorModem4G per enviar
 * SMS i rebre'n via l'API REST del Huawei E3372 HiLink (http://192.168.8.1).
 * La recepció es fa per polling cada 30 segons.
 *
 * @see IServeiMissatgeria, AdaptadorModem4G, GestorCanals
 */
class ServeiSMS : public IServeiMissatgeria
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit ServeiSMS(QObject *parent = nullptr);

    /**
     * @brief Envia un SMS al contacte via el Huawei HiLink.
     * @param m Missatge a enviar (s'usa m->text).
     * @param c Contacte destinatari (s'usa c->numeroTelefon).
     * @return true si el Huawei ha confirmat l'enviament.
     */
    bool enviar(Missatge *m, Contacte *c) override;

    /**
     * @brief Obté els SMS rebuts (no usat; la recepció és per polling via adaptador).
     * @return Llista buida (la recepció s'emet via signal smsRebut de l'adaptador).
     */
    QList<Missatge*> rebre() override;

    /**
     * @brief Obté la llista de contactes SMS (no implementat).
     * @return Llista buida.
     */
    QList<Contacte*> obtenirContactes() override;

    /**
     * @brief Comprova si el Huawei HiLink respon a la xarxa local.
     * @return true si http://192.168.8.1 és accessible.
     */
    bool estaDisponible() override;

    /**
     * @brief Retorna l'adaptador intern per connectar el signal smsRebut.
     * @return Punter a l'AdaptadorModem4G.
     */
    AdaptadorModem4G* adaptador() const { return m_adaptador; }

private:
    AdaptadorModem4G *m_adaptador; ///< Adaptador del mòdem 4G Huawei HiLink
};

#endif // SERVEISMS_H
