#ifndef SERVEIAUDIO_H
#define SERVEIAUDIO_H

#include <QObject>
#include <QString>

class ServeiTranscripcio;
class AdaptadorDispositiuAudio;

/**
 * @brief Servei de gravació i transcripció d'àudio. Patró Strategy.
 *
 * Gestiona la gravació de veu mitjançant l'AdaptadorDispositiuAudio
 * i la transcripció a text via ServeiTranscripcio.
 *
 * @see ServeiTranscripcio, AdaptadorDispositiuAudio, ControladorAplicacio
 */
class ServeiAudio : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit ServeiAudio(QObject *parent = nullptr);

    /** @brief Inicia la gravació d'àudio. @return true si ha iniciat correctament. */
    bool iniciarGravacio();

    /**
     * @brief Atura la gravació i retorna el text transcrit.
     * @return Text transcrit de l'àudio gravat.
     */
    QString aturarGravacio();

private:
    ServeiTranscripcio       *transcriptor; ///< Servei de transcripció d'àudio a text
    AdaptadorDispositiuAudio *m_adaptador;  ///< Adaptador del micròfon/altaveu
};

#endif // SERVEIAUDIO_H
