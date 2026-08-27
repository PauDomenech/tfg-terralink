#include "missatge.h"
#include <QUuid>

Missatge::Missatge(QObject *parent)
    : QObject(parent)
    , estat(EstatMissatge::Esborrany)
{
    id = QUuid::createUuid().toString();
    dataCreacio = QDateTime::currentDateTime();
}

void Missatge::marcarEnviat()
{
    estat = EstatMissatge::Enviat;
}

void Missatge::marcarRebut()
{
    estat = EstatMissatge::Rebut;
}

void Missatge::marcarFallit()
{
    estat = EstatMissatge::Fallit;
}
