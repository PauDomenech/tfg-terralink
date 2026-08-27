#ifndef ADAPTADORDISPOSITIUAUDIO_H
#define ADAPTADORDISPOSITIUAUDIO_H

#include <QObject>
#include <QString>
#include <QProcess>

/**
 * @brief Adaptador per al dispositiu d'àudio (micròfon).
 *
 * Gestiona la gravació d'àudio del micròfon de la Raspberry Pi
 * mitjançant la comanda `arecord` (ALSA) en format WAV 16kHz mono,
 * òptim per a la transcripció amb Whisper.
 *
 * @see ServeiAudio, ServeiTranscripcio
 */
class AdaptadorDispositiuAudio : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit AdaptadorDispositiuAudio(QObject *parent = nullptr);

    /** @brief Inicia la gravació d'àudio. @return true si ha iniciat correctament. */
    bool iniciarGravacio();

    /**
     * @brief Atura la gravació i retorna la ruta al fitxer gravat.
     * @return Ruta al fitxer WAV gravat.
     */
    QString aturarGravacio();

    /** @brief Comprova si arecord està disponible al sistema. */
    bool estaDisponible();

private:
    QString   m_rutaFitxer; ///< Ruta on es desa el fitxer d'àudio gravat
    QProcess *m_proces;     ///< Procés arecord actiu
};

#endif // ADAPTADORDISPOSITIUAUDIO_H
