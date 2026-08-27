#include "gestoremergencia.h"
#include "iserveimissatgeria.h"
#include <QDebug>

GestorEmergencia::GestorEmergencia(QObject *parent)
    : QObject(parent)
    , monitor(nullptr)
{
}

void GestorEmergencia::enviarEmergencia(Missatge *m, Contacte *c)
{
    // Broadcast simultani per tots els canals disponibles
    int fallits = 0;

    for (IServeiMissatgeria *canal : canalsOrdenats) {
        if (!canal->estaDisponible()) {
            fallits++;
            qDebug() << "Canal no disponible:" << canal;
            emit canalFallit(canal);
            continue;
        }

        bool enviat = canal->enviar(m, c);

        if (enviat) {
            qDebug() << "Canal enviat OK:" << canal;
            emit canalEnviatOk(canal);
        } else {
            fallits++;
            qDebug() << "Canal fallat:" << canal;
            emit canalFallit(canal);
        }
    }

    qDebug() << "Emergència completada. Fallits:" << fallits;
    emit totsCanalsCompletats(fallits == 0, fallits);
}

IServeiMissatgeria* GestorEmergencia::seleccionarMillorCanal()
{
    for (IServeiMissatgeria *canal : canalsOrdenats) {
        if (canal->estaDisponible())
            return canal;
    }
    return nullptr;
}

void GestorEmergencia::onTimeout()
{
    // TODO: marcar canal com a fallat per timeout
}
