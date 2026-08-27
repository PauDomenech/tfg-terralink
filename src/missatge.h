#ifndef MISSATGE_H
#define MISSATGE_H

#include <QObject>
#include <QDateTime>

/**
 * @brief Estats possibles d'un missatge.
 */
enum class EstatMissatge {
    Esborrany,  ///< Missatge creat però no enviat
    Enviant,    ///< Missatge en procés d'enviament
    Enviat,     ///< Missatge enviat correctament
    Rebut,      ///< Missatge rebut pel destinatari
    Fallit,     ///< Enviament fallit
    Impres      ///< Missatge imprès per la impressora
};

/**
 * @brief Classe abstracta base per a tots els tipus de missatge.
 *
 * Conté les dades comunes a tots els missatges (text, data, estat, canal).
 * Les subclasses concretes (MissatgeWhatsApp, MissatgeSMS, MissatgeLoRa)
 * implementen els mètodes virtuals purs.
 *
 * @see MissatgeWhatsApp, MissatgeSMS, MissatgeLoRa, MissatgeFactory
 */
class Missatge : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit Missatge(QObject *parent = nullptr);

    QString       id;           ///< Identificador únic del missatge (UUID)
    QDateTime     dataCreacio;  ///< Data i hora de creació
    QString       text;         ///< Contingut del missatge
    QString       rutaAudio;    ///< Ruta al fitxer d'àudio (si escau)
    EstatMissatge estat;        ///< Estat actual del missatge
    QString       canal;        ///< Canal d'enviament ("whatsapp", "sms", "lora")

    /** @brief Serialitza el missatge a text per enviar-lo. */
    virtual QString serialitzar() const = 0;

    /** @brief Retorna el nom del canal d'aquest missatge. */
    virtual QString canal_() const = 0;

    /** @brief Marca el missatge com a enviat. */
    void marcarEnviat();

    /** @brief Marca el missatge com a rebut. */
    void marcarRebut();

    /** @brief Marca el missatge com a fallit. */
    void marcarFallit();
};

#endif // MISSATGE_H
