#include "adaptadordispositiuaudio.h"

#include <QProcess>
#include <QFile>
#include <QDebug>

AdaptadorDispositiuAudio::AdaptadorDispositiuAudio(QObject *parent)
    : QObject(parent)
    , m_rutaFitxer("/tmp/terralink_audio.wav")
    , m_proces(nullptr)
{}

bool AdaptadorDispositiuAudio::iniciarGravacio()
{
    if (m_proces && m_proces->state() != QProcess::NotRunning) {
        qDebug() << "[Audio] Ja s'està gravant";
        return false;
    }

    m_proces = new QProcess(this);

    // arecord: grava en format WAV, 16kHz mono (òptim per Whisper)
    m_proces->start("arecord", {
                                   "-D", "hw:3,0",   // ← dispositiu USB
                                   "-f", "S16_LE",   // format 16-bit
                                   "-r", "16000",    // 16kHz (òptim per Whisper)
                                   "-c", "1",        // mono
                                   m_rutaFitxer
                               });

    if (!m_proces->waitForStarted(3000)) {
        qDebug() << "[Audio] No s'ha pogut iniciar arecord";
        return false;
    }

    qDebug() << "[Audio] Gravació iniciada";
    return true;
}

QString AdaptadorDispositiuAudio::aturarGravacio()
{
    if (!m_proces || m_proces->state() == QProcess::NotRunning) {
        qDebug() << "[Audio] No hi ha gravació activa";
        return QString();
    }

    // Aturar arecord
    m_proces->terminate();
    m_proces->waitForFinished(3000);
    m_proces->deleteLater();
    m_proces = nullptr;

    qDebug() << "[Audio] Gravació aturada, fitxer:" << m_rutaFitxer;
    return m_rutaFitxer;
}

bool AdaptadorDispositiuAudio::estaDisponible()
{
    // Comprovar si arecord existeix al sistema
    QProcess proc;
    proc.start("which", {"arecord"});
    proc.waitForFinished(1000);
    return proc.exitCode() == 0;
}
