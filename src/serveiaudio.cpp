#include "serveiaudio.h"
#include "serveitranscripcio.h"
#include "adaptadordispositiuaudio.h"

ServeiAudio::ServeiAudio(QObject *parent)
    : QObject(parent)
{
    //transcriptor = new ServeiTranscripcio(this);
    m_adaptador  = new AdaptadorDispositiuAudio(this);
}

bool ServeiAudio::iniciarGravacio()
{
    return m_adaptador->iniciarGravacio();
}

QString ServeiAudio::aturarGravacio()
{
    QString ruta = m_adaptador->aturarGravacio();
    // NO transcribim aquí — ho fa EnviarMissatge de forma asíncrona
    return ruta;
}
