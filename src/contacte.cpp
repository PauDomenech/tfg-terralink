#include "contacte.h"
#include <QUuid>

Contacte::Contacte(QObject *parent)
    : QObject(parent)
{
    id = QUuid::createUuid().toString();
}
