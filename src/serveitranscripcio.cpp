#include "serveitranscripcio.h"

#include <QProcess>
#include <QDebug>

QString ServeiTranscripcio::transcriure(const QString &rutaAudio)
{
    if (rutaAudio.isEmpty()) {
        qDebug() << "[Transcripcio] Ruta d'àudio buida";
        return QString();
    }

    QProcess proces;
    proces.start("python3", {m_rutaScript, rutaAudio});

    // Esperem fins a 60 segons (Whisper pot trigar a la Raspberry Pi)
    if (!proces.waitForFinished(60000)) {
        qDebug() << "[Transcripcio] Timeout esperant Whisper";
        proces.kill();
        return QString();
    }

    if (proces.exitCode() != 0) {
        QString error = QString::fromUtf8(proces.readAllStandardError());
        qDebug() << "[Transcripcio] Error de Whisper:" << error;
        return QString();
    }

    QString text = QString::fromUtf8(proces.readAllStandardOutput()).trimmed();
    qDebug() << "[Transcripcio] Text transcrit:" << text;
    return text;
}
