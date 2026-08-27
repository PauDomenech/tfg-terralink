#include "adaptadorimpressoraescpos.h"

#include <QFile>
#include <QStringEncoder>
#include <QDateTime>
#include <QDebug>
#include <QImage>
#include <QColor>

const QByteArray AdaptadorImpressoraEscPos::CMD_INIT        = QByteArray("\x1B\x40", 2);
const QByteArray AdaptadorImpressoraEscPos::CMD_NEGRETA_ON  = QByteArray("\x1B\x45\x01", 3);
const QByteArray AdaptadorImpressoraEscPos::CMD_NEGRETA_OFF = QByteArray("\x1B\x45\x00", 3);
const QByteArray AdaptadorImpressoraEscPos::CMD_TALL        = QByteArray("\x1D\x56\x42\x00", 4);
const QByteArray AdaptadorImpressoraEscPos::CMD_ALIMENTAR   = QByteArray("\x1B\x64\x02", 3); // només 2 línies

AdaptadorImpressoraEscPos::AdaptadorImpressoraEscPos(QObject *parent)
    : QObject(parent)
    , m_port("/dev/usb/lp0")
{}

bool AdaptadorImpressoraEscPos::escriureAlPort(const QByteArray &dades)
{
    QFile port(m_port);
    if (!port.open(QIODevice::WriteOnly)) {
        qDebug() << "[Impressora] No es pot obrir el port:" << m_port;
        return false;
    }
    port.write(dades);
    port.close();
    return true;
}

bool AdaptadorImpressoraEscPos::imprimirCapcalera()
{
    QStringEncoder encoder(QStringEncoder::Latin1);
    QByteArray dades;

    QString dataHora = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    dades += CMD_INIT;
    // Seleccionar pàgina de codis CP850 (espanyol/català)
    dades += QByteArray("\x1B\x74\x10", 3); // ESC t 2 — CP850
    dades += CMD_NEGRETA_ON;
    dades += encoder.encode(QString("--- TerraLink ---\n"));
    dades += CMD_NEGRETA_OFF;
    dades += encoder.encode(dataHora + "\n");
    dades += CMD_ALIMENTAR;

    return escriureAlPort(dades);
}

bool AdaptadorImpressoraEscPos::imprimirMissatgeFormat(const QString &canal,
                                                       const QDateTime &hora,
                                                       const QString &text,
                                                       bool mostrarCanal)
{

    Q_UNUSED(hora)
    QStringEncoder encoder(QStringEncoder::Latin1);
    QByteArray dades;

    // Si el canal ha canviat, imprimim la capçalera del canal
    if (mostrarCanal) {
        dades += CMD_NEGRETA_ON;
        dades += encoder.encode(canal.toUpper() + ":\n");
        dades += CMD_NEGRETA_OFF;
    }

    dades += encoder.encode(text + "\n\n");

    return escriureAlPort(dades);
}

bool AdaptadorImpressoraEscPos::imprimir(const QString &nom, const QString &text)
{
    Q_UNUSED(nom) Q_UNUSED(text)
    return false;
}

bool AdaptadorImpressoraEscPos::estaDisponible()
{
    QFile port(m_port);
    return port.exists();
}

bool AdaptadorImpressoraEscPos::imprimirImatge(const QString &base64)
{
    // Decodifiquem el base64 a bytes
    QByteArray imatgeBytes = QByteArray::fromBase64(base64.toUtf8());

    // Redimensionar a 384px d'ample (ample estàndard impressores 80mm)
    QImage imatge;
    if (!imatge.loadFromData(imatgeBytes)) {
        qDebug() << "[Impressora] No s'ha pogut carregar la imatge";
        return false;
    }

    imatge = imatge.scaledToWidth(384, Qt::SmoothTransformation)
                 .convertToFormat(QImage::Format_Mono);

    // Convertir a format ESC/POS bitmap
    QByteArray dades;
    int amplada = imatge.width();
    int alcada  = imatge.height();
    int bytesPerFila = (amplada + 7) / 8;

    // GS v 0 — imprimir imatge en mode ràster
    dades += QByteArray("\x1D\x76\x30\x00", 4);
    dades += (char)(bytesPerFila & 0xFF);
    dades += (char)((bytesPerFila >> 8) & 0xFF);
    dades += (char)(alcada & 0xFF);
    dades += (char)((alcada >> 8) & 0xFF);

    for (int y = 0; y < alcada; y++) {
        for (int x = 0; x < bytesPerFila; x++) {
            char byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                int px = x * 8 + bit;
                if (px < amplada) {
                    QColor color = imatge.pixelColor(px, y);
                    if (color.lightness() < 128)
                        byte |= (1 << (7 - bit));
                }
            }
            dades += byte;
        }
    }

    dades += "\n";
    return escriureAlPort(dades);
}

bool AdaptadorImpressoraEscPos::imprimirImatgeDesDeRuta(const QString &ruta)
{
    QImage imatge(ruta);
    if (imatge.isNull()) {
        qDebug() << "[Impressora] No s'ha pogut carregar la imatge:" << ruta;
        return false;
    }

    imatge = imatge.scaledToWidth(384, Qt::SmoothTransformation)
                 .convertToFormat(QImage::Format_Mono);

    int amplada     = imatge.width();
    int alcada      = imatge.height();
    int bytesPerFila = (amplada + 7) / 8;

    QByteArray dades;

    // GS v 0 — imprimir imatge en mode ràster
    dades += QByteArray("\x1D\x76\x30\x00", 4);
    dades += (char)(bytesPerFila & 0xFF);
    dades += (char)((bytesPerFila >> 8) & 0xFF);
    dades += (char)(alcada & 0xFF);
    dades += (char)((alcada >> 8) & 0xFF);

    for (int y = 0; y < alcada; y++) {
        for (int x = 0; x < bytesPerFila; x++) {
            char byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                int px = x * 8 + bit;
                if (px < amplada) {
                    if (imatge.pixelColor(px, y).lightness() < 128)
                        byte |= (1 << (7 - bit));
                }
            }
            dades += byte;
        }
    }

    dades += "\n\n";
    return escriureAlPort(dades);
}
