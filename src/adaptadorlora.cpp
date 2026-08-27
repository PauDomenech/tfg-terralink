#include "adaptadorlora.h"

AdaptadorLoRa::AdaptadorLoRa(QObject *parent)
    : QObject(parent)
    , m_port("/dev/ttyAMA0")
{}

bool AdaptadorLoRa::enviar(const QString &nodeDestinacio, const QString &payload)
{
    Q_UNUSED(nodeDestinacio) Q_UNUSED(payload)
    // TODO: enviar paquet via Meshtastic/LoRa per SPI o UART
    return false;
}

bool AdaptadorLoRa::estaDisponible()
{
    // TODO: comprovar si el mòdul LoRa respon
    return false;
}
