#include "adaptadorwhatsapp.h"

#include <QEventLoop>
#include <QTimer>
#include <QDebug>

AdaptadorWhatsApp::AdaptadorWhatsApp(QObject *parent)
    : QObject(parent)
{
    m_proces = new QProcess(this);
    m_proces->setProgram("node");
    m_proces->setArguments({m_rutaScript});
    m_proces->setWorkingDirectory(m_directoriTreball);
    m_proces->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_proces, &QProcess::readyReadStandardOutput,
            this, &AdaptadorWhatsApp::enLlegirSortida);
    connect(m_proces, &QProcess::stateChanged,
            this, &AdaptadorWhatsApp::enCanviarEstat);
}

AdaptadorWhatsApp::~AdaptadorWhatsApp()
{
    aturarProces();
}

void AdaptadorWhatsApp::iniciarProces()
{
    if (m_proces->state() != QProcess::NotRunning)
        return;
    m_proces->start();
}

void AdaptadorWhatsApp::aturarProces()
{
    if (m_proces->state() == QProcess::NotRunning)
        return;
    m_proces->terminate();
    if (!m_proces->waitForFinished(2500))
        m_proces->kill();
}

void AdaptadorWhatsApp::enCanviarEstat(QProcess::ProcessState estat)
{
    qDebug() << "[WhatsApp] Estat procés:" << estat;
}

void AdaptadorWhatsApp::enLlegirSortida()
{
    QString text = QString::fromUtf8(m_proces->readAllStandardOutput());

    for (const QString &lineaBruta : text.split('\n')) {
        QString linea = lineaBruta;

        // Captura del QR ASCII art (bloc multilínia)
        if (linea.trimmed() == "QR_ASCII_START") {
            m_capturantQr = true;
            m_bufferQr.clear();
            continue;
        }
        if (linea.trimmed() == "QR_ASCII_END") {
            m_capturantQr = false;
            emit qrRebut(m_bufferQr);
            continue;
        }
        if (m_capturantQr) {
            m_bufferQr += linea + "\n";
            continue;
        }

        QString lineaNeta = linea.trimmed();
        if (lineaNeta.isEmpty()) continue;

        if (lineaNeta == "APP_READY") {
            m_preparat = true;
            emit connectat();
        }
        else if (lineaNeta == "APP_DISCONNECTED") {
            m_preparat = false;
            emit desconnectat();
        }
        else if (lineaNeta.startsWith("INCOMING:")) {
            emit missatgeRebut(lineaNeta.mid(9));
        }
        else if (lineaNeta == "SEND_OK") {
            m_ultimEnviamentOk = true;
            m_esperantResposta = false;
        }
        else if (lineaNeta == "SEND_ERROR") {
            m_ultimEnviamentOk = false;
            m_esperantResposta = false;
        }
        else if (lineaNeta == "CONTACTS_START") {
            m_capturantContactes = true;
            m_bufferContactes.clear();
        }
        else if (m_capturantContactes && lineaNeta == "CONTACTS_END") {
            m_capturantContactes = false;
            qDebug() << "[WhatsApp] Contactes rebuts, mida:" << m_bufferContactes.size();
            emit contactesRebuts(m_bufferContactes);
        }
        else if (m_capturantContactes) {
            m_bufferContactes += lineaNeta;
        }
        else if (lineaNeta == "CONTACTS_ERROR") {
            qDebug() << "[WhatsApp] Error obtenint contactes";
        }
        else {
            qDebug() << "[WhatsApp]" << lineaNeta;
        }
    }
}

void AdaptadorWhatsApp::demanarContactes()
{
    if (!m_preparat) return;
    m_proces->write("GET_CONTACTS\n");
    m_proces->waitForBytesWritten(1000);
}

bool AdaptadorWhatsApp::enviarWhatsApp(const QString &numero, const QString &text)
{
    if (!m_preparat) return false;

    QString comanda = QString("SEND:%1:%2\n").arg(numero, text);
    m_proces->write(comanda.toUtf8());
    m_proces->waitForBytesWritten(1000);

    m_esperantResposta = true;
    m_ultimEnviamentOk = false;

    // Espera SEND_OK / SEND_ERROR amb polling cada 100ms (màx 5s)
    QEventLoop bucle;
    QTimer temporitzador;
    temporitzador.setSingleShot(true);
    temporitzador.setInterval(5000);
    connect(&temporitzador, &QTimer::timeout, &bucle, &QEventLoop::quit);

    QTimer *comprovador = new QTimer(this);
    comprovador->setInterval(100);
    connect(comprovador, &QTimer::timeout, [&]() {
        if (!m_esperantResposta)
            bucle.quit();
    });

    temporitzador.start();
    comprovador->start();
    bucle.exec();

    comprovador->stop();
    comprovador->deleteLater();

    return m_ultimEnviamentOk;
}

bool AdaptadorWhatsApp::estaDisponible()
{
    return m_preparat;
}
