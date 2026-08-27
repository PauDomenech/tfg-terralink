#ifndef CONTROLADORAPLICACIO_H
#define CONTROLADORAPLICACIO_H

#include <QObject>

class GestorEmergencia;
class ServeiAudio;
class ServeiImpressio;
class Missatge;
class Contacte;

/**
 * @brief Controlador principal de l'aplicació. Patró Singleton.
 *
 * Coordina totes les capes del sistema: gestiona l'enviament de missatges,
 * l'activació d'emergències i la transcripció d'àudio.
 * Accedeix als repositoris i als serveis a través del GestorCanals.
 *
 * @see GestorEmergencia, GestorCanals, MissatgeFactory,
 *      RepositoriContactes, RepositoriMissatges
 */
class ControladorAplicacio : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Retorna la instància única del controlador (Singleton).
     * @return Referència a la instància única.
     */
    static ControladorAplicacio& instancia();

    /** @brief Inicialitza el sistema: canals, monitor, impressora i contactes de prova. */
    void inicialitzar();

    /**
     * @brief Envia un missatge de text per tots els canals disponibles.
     * @param text Text del missatge a enviar.
     */
    void enviarMissatgeText(const QString &text);

    /**
     * @brief Atura la gravació d'àudio, el transcriu i l'envia al primer contacte.
     */
    void transcriureAudio();

    /**
     * @brief Processa un missatge entrant: el desa al repositori i l'imprimeix.
     * @param m Missatge rebut (WhatsApp, SMS o LoRa).
     */
    void processarMissatgeEntrant(Missatge *m);

    /**
     * @brief Activa el protocol d'emergència per a tots els contactes marcats.
     * @param c Paràmetre ignorat; s'envia a tots els contactes amb esEmergencia = true.
     */
    void activarEmergencia(Contacte *c);

    /**
     * @brief Inicia la gravació de veu via ServeiAudio.
     * @return true si la gravació ha iniciat correctament.
     */
    bool iniciarGravacio();

    /**
     * @brief Atura la gravació, transcriu i envia el missatge a un contacte específic.
     * @param c Contacte destinatari del missatge transcrit.
     */
    void transcriureIEnviaA(Contacte *c);

    /**
     * @brief Atura la gravació i retorna la ruta al fitxer d'àudio (per transcripció asíncrona).
     * @return Ruta al fitxer WAV gravat.
     */
    QString aturarITranscriure();

    /**
     * @brief Envia un text a un contacte específic per WhatsApp.
     * @param text Text a enviar.
     * @param c Contacte destinatari.
     */
    void enviarTextAContacte(const QString &text, Contacte *c);

    GestorEmergencia *gestorEmergencia; ///< Gestor del protocol d'emergència (broadcast multicanal)

private slots:
    /**
     * @brief S'executa quan arriba un missatge de WhatsApp via AdaptadorWhatsApp.
     * @param contingut Text del missatge (pot incloure [FOTO] i ruta d'imatge).
     */
    void onMissatgeWhatsAppRebut(const QString &contingut);

    /**
     * @brief S'executa quan el polling detecta un SMS nou via AdaptadorModem4G.
     * @param telefon Número de telèfon del remitent.
     * @param text Cos del missatge SMS.
     * @param data Data i hora d'enviament.
     */
    void onSMSRebut(const QString &telefon, const QString &text, const QString &data);

private:
    ControladorAplicacio(QObject *parent = nullptr);
    ControladorAplicacio(const ControladorAplicacio&) = delete;
    ControladorAplicacio& operator=(const ControladorAplicacio&) = delete;

    ServeiAudio     *m_serveiAudio;     ///< Servei de gravació i transcripció d'àudio
    ServeiImpressio *m_serveiImpressio; ///< Servei d'impressió tèrmica ESC/POS
};

#endif // CONTROLADORAPLICACIO_H
