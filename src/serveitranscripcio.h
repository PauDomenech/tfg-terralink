#ifndef SERVEITRANSCRIPCIO_H
#define SERVEITRANSCRIPCIO_H

#include <QString>

/**
 * @brief Servei de transcripció d'àudio a text mitjançant Whisper.
 *
 * Executa el script Python `transcriure.py` via QProcess per
 * transcriure fitxers d'àudio WAV en local, sense connexió a internet.
 * Suporta català, castellà i detecció automàtica d'idioma.
 *
 * @see ServeiAudio
 */
class ServeiTranscripcio
{
public:
    ServeiTranscripcio() = default;

    /**
     * @brief Transcriu un fitxer d'àudio a text.
     * @param rutaAudio Ruta al fitxer d'àudio WAV.
     * @return Text transcrit, o string buit si hi ha error.
     */
    QString transcriure(const QString &rutaAudio);

private:
    QString m_rutaScript = "/home/paudomenech/TerraLink/transcriure.py"; ///< Ruta al script Python
};

#endif // SERVEITRANSCRIPCIO_H
