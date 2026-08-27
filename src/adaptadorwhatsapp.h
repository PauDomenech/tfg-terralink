#ifndef ADAPTADORWHATSAPP_H
#define ADAPTADORWHATSAPP_H

#include <QObject>
#include <QString>
#include <QProcess>

/**
 * @brief Adaptador per a la integració amb WhatsApp via Node.js. Patró Adapter.
 *
 * Gestiona el procés Node.js (whatsapp.js) mitjançant QProcess.
 * La comunicació es fa via stdin/stdout:
 * - Qt envia comandes: @c SEND:numero:text, @c GET_CONTACTS, @c EMERGENCY
 * - Node respon: @c SEND_OK, @c SEND_ERROR, @c APP_READY, @c APP_DISCONNECTED,
 *   @c QR_ASCII_START/END, @c CONTACTS_START/END, @c INCOMING:
 *
 * @see ServeiWhatsApp, AdaptadorModem4G
 */
class AdaptadorWhatsApp : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit AdaptadorWhatsApp(QObject *parent = nullptr);
    ~AdaptadorWhatsApp();

    /** @brief Inicia el procés Node.js de WhatsApp. */
    void iniciarProces();

    /** @brief Atura el procés Node.js de WhatsApp de forma segura. */
    void aturarProces();

    /**
     * @brief Envia un missatge de WhatsApp via el procés Node.
     * @param numero Número de telèfon destinatari (amb prefix país, sense +).
     * @param text Text del missatge.
     * @return true si Node ha confirmat l'enviament (SEND_OK).
     */
    bool enviarWhatsApp(const QString &numero, const QString &text);

    /**
     * @brief Comprova si WhatsApp està connectat i llest.
     * @return true si el procés Node ha arribat a APP_READY.
     */
    bool estaDisponible();

    /** @brief Demana la llista de contactes al procés Node (GET_CONTACTS). */
    void demanarContactes();

signals:
    /** @brief S'emet quan Node genera un QR en format ASCII art. */
    void qrRebut(const QString &qr);

    /** @brief S'emet quan WhatsApp s'ha connectat correctament (APP_READY). */
    void connectat();

    /** @brief S'emet quan WhatsApp es desconnecta (APP_DISCONNECTED). */
    void desconnectat();

    /**
     * @brief S'emet quan arriba un missatge entrant (INCOMING).
     * @param contingut Text del missatge rebut (pot incloure [FOTO] i ruta).
     */
    void missatgeRebut(const QString &contingut);

    /**
     * @brief S'emet quan s'ha rebut la llista de contactes (CONTACTS_END).
     * @param json JSON amb la llista de contactes de WhatsApp.
     */
    void contactesRebuts(const QString &json);

private slots:
    /** @brief Llegeix i processa la sortida del procés Node línia per línia. */
    void enLlegirSortida();

    /** @brief Registra els canvis d'estat del procés Node. */
    void enCanviarEstat(QProcess::ProcessState estat);

private:
    QProcess *m_proces;                     ///< Procés Node.js de WhatsApp
    bool      m_preparat = false;           ///< true quan Node arriba a APP_READY
    bool      m_capturantQr = false;        ///< true mentre captura línies del QR ASCII
    QString   m_bufferQr;                   ///< Buffer acumulador del QR ASCII art
    bool      m_capturantContactes = false; ///< true mentre captura el JSON de contactes
    QString   m_bufferContactes;            ///< Buffer acumulador del JSON de contactes
    bool      m_esperantResposta = false;   ///< true mentre espera SEND_OK/SEND_ERROR
    bool      m_ultimEnviamentOk = false;   ///< Resultat de l'últim enviament
    QString   m_rutaScript = "whatsapp.js"; ///< Nom del script Node.js
    QString   m_directoriTreball = "/home/paudomenech/TerraLink"; ///< Directori de treball
};

#endif // ADAPTADORWHATSAPP_H
