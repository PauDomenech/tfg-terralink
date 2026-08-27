#ifndef ADAPTADORMODEM4G_H
#define ADAPTADORMODEM4G_H

#include <QObject>
#include <QString>
#include <QTimer>

/**
 * @brief Adaptador per al mòdem 4G Huawei E3372 HiLink. Patró Adapter.
 *
 * Gestiona l'enviament i recepció de SMS via l'API REST del Huawei HiLink
 * accessible a http://192.168.8.1/api/. Fa polling periòdic cada 30 segons
 * per detectar SMS entrants nous i els emet via el signal smsRebut.
 *
 * Protocol intern:
 * - Obté token CSRF i sessió de @c /api/webserver/SesTokInfo
 * - Envia SMS via POST a @c /api/sms/send-sms
 * - Llista SMS via POST a @c /api/sms/sms-list
 *
 * @see ServeiSMS, AdaptadorWhatsApp
 */
class AdaptadorModem4G : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit AdaptadorModem4G(QObject *parent = nullptr);

    /**
     * @brief Envia un SMS via l'API Huawei HiLink.
     * @param numero Número de telèfon destinatari (sense +, ex: 34659794561).
     * @param text Text del missatge SMS.
     * @return true si el Huawei ha confirmat l'enviament amb <response>OK</response>.
     */
    bool enviarSMS(const QString &numero, const QString &text);

    /**
     * @brief Comprova si el Huawei respon a la xarxa local.
     * @return true si http://192.168.8.1/api/device/information és accessible.
     */
    bool estaDisponibleSMS();

    /** @brief Inicia el polling periòdic de SMS cada 30 segons. */
    void iniciarPollingSMS();

    /** @brief Atura el polling de SMS i allibera el timer. */
    void aturarPollingSMS();

signals:
    /**
     * @brief S'emet quan s'ha detectat un SMS nou durant el polling.
     * @param telefon Número de telèfon del remitent.
     * @param text Cos del missatge SMS.
     * @param data Data i hora d'enviament (format Huawei).
     */
    void smsRebut(const QString &telefon, const QString &text, const QString &data);

private slots:
    /** @brief Comprova si hi ha SMS nous al Huawei HiLink. Cridat pel timer. */
    void comprovarSMSNous();

private:
    /**
     * @brief Obté el token CSRF i la sessió del Huawei HiLink.
     *
     * No depèn de cap estat de la instància (només fa una petició HTTP
     * i omple els paràmetres de sortida), per això és static.
     * @param token [out] Token CSRF retornat.
     * @param sessio [out] SessionID retornat.
     * @return true si s'han obtingut correctament (ambdós no buits).
     */
    static bool obtenirTokenISessio(QString &token, QString &sessio);

    QTimer *m_timerSMS = nullptr;       ///< Timer de polling SMS cada 30 segons
    int     m_ultimIndexSMS = -1;       ///< Índex de l'últim SMS processat
    bool    m_primeraPollingSMS = true; ///< true fins la primera comprovació (no emet signals)
};

#endif // ADAPTADORMODEM4G_H
