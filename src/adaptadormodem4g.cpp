#include "adaptadormodem4g.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QUrl>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QDebug>

AdaptadorModem4G::AdaptadorModem4G(QObject *parent)
    : QObject(parent)
{}

bool AdaptadorModem4G::obtenirTokenISessio(QString &token, QString &sessio)
{
    QNetworkAccessManager xarxa;
    QNetworkRequest peticio(QUrl("http://192.168.8.1/api/webserver/SesTokInfo"));
    QNetworkReply *resposta = xarxa.get(peticio);

    QEventLoop bucle;
    connect(resposta, &QNetworkReply::finished, &bucle, &QEventLoop::quit);
    QTimer::singleShot(5000, &bucle, &QEventLoop::quit);
    bucle.exec();

    if (resposta->error() != QNetworkReply::NoError) {
        qDebug() << "[SMS] Error obtenint token:" << resposta->errorString();
        resposta->deleteLater();
        return false;
    }

    QString xml = QString::fromUtf8(resposta->readAll());
    resposta->deleteLater();

    QRegularExpression rexToken("<TokInfo>([^<]+)</TokInfo>");
    QRegularExpression rexSessio("<SesInfo>([^<]+)</SesInfo>");
    token  = rexToken.match(xml).captured(1);
    sessio = rexSessio.match(xml).captured(1);

    if (token.isEmpty() || sessio.isEmpty()) {
        qDebug() << "[SMS] Token o sessió buits";
        return false;
    }
    return true;
}

bool AdaptadorModem4G::enviarSMS(const QString &numero, const QString &text)
{
    QString token, sessio;
    if (!obtenirTokenISessio(token, sessio))
        return false;

    qDebug() << "[SMS] Enviant a:" << numero;

    QString dataHora = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString xmlBody = QString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<request>"
        "<Index>-1</Index>"
        "<Phones><Phone>+%1</Phone></Phones>"
        "<Sca></Sca>"
        "<Content>%2</Content>"
        "<Length>%3</Length>"
        "<Reserved>1</Reserved>"
        "<Date>%4</Date>"
        "</request>"
    ).arg(numero, text, QString::number(text.length()), dataHora);

    QNetworkAccessManager xarxa;
    QNetworkRequest peticio(QUrl("http://192.168.8.1/api/sms/send-sms"));
    peticio.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
    peticio.setRawHeader("__RequestVerificationToken", token.toUtf8());
    peticio.setRawHeader("Cookie", sessio.toUtf8());

    QNetworkReply *resposta = xarxa.post(peticio, xmlBody.toUtf8());

    QEventLoop bucle;
    connect(resposta, &QNetworkReply::finished, &bucle, &QEventLoop::quit);
    QTimer::singleShot(10000, &bucle, &QEventLoop::quit);
    bucle.exec();

    QString respostaXml = QString::fromUtf8(resposta->readAll());
    resposta->deleteLater();

    bool ok = respostaXml.contains("<response>OK</response>");
    qDebug() << "[SMS] Resultat:" << (ok ? "OK" : "ERROR") << respostaXml;
    return ok;
}

bool AdaptadorModem4G::estaDisponibleSMS()
{
    QNetworkAccessManager xarxa;
    QNetworkRequest peticio(QUrl("http://192.168.8.1/api/device/information"));
    QNetworkReply *resposta = xarxa.get(peticio);

    QEventLoop bucle;
    connect(resposta, &QNetworkReply::finished, &bucle, &QEventLoop::quit);
    QTimer::singleShot(3000, &bucle, &QEventLoop::quit);
    bucle.exec();

    // Si ha sortit del bucle per timeout (sense que la petició hagi acabat),
    // resposta->error() encara val NoError per defecte — cal comprovar isFinished()
    bool disponible = resposta->isFinished() && (resposta->error() == QNetworkReply::NoError);

    if (!resposta->isFinished())
        resposta->abort(); // netegem la petició pendent que ha fet timeout

    resposta->deleteLater();

    qDebug() << "[SMS] Huawei disponible:" << disponible;
    return disponible;
}

void AdaptadorModem4G::iniciarPollingSMS()
{
    if (m_timerSMS) return;

    m_timerSMS = new QTimer(this);
    m_timerSMS->setInterval(30000); // cada 30 segons
    connect(m_timerSMS, &QTimer::timeout, this, &AdaptadorModem4G::comprovarSMSNous);
    m_timerSMS->start();

    // Primera comprovació immediata per establir l'índex inicial sense emetre signals
    comprovarSMSNous();
    qDebug() << "[SMS] Polling iniciat";
}

void AdaptadorModem4G::aturarPollingSMS()
{
    if (m_timerSMS) {
        m_timerSMS->stop();
        m_timerSMS->deleteLater();
        m_timerSMS = nullptr;
    }
}

void AdaptadorModem4G::comprovarSMSNous()
{
    QString token, sessio;
    if (!obtenirTokenISessio(token, sessio)) return;

    // Demanar els últims 20 SMS de la safata d'entrada
    QString xmlPeticio =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<request>"
        "<PageIndex>1</PageIndex>"
        "<ReadCount>20</ReadCount>"
        "<BoxType>1</BoxType>"
        "<SortType>0</SortType>"
        "<Ascending>0</Ascending>"
        "<UnreadPreferred>1</UnreadPreferred>"
        "</request>";

    QNetworkAccessManager xarxa;
    QNetworkRequest peticio(QUrl("http://192.168.8.1/api/sms/sms-list"));
    peticio.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
    peticio.setRawHeader("__RequestVerificationToken", token.toUtf8());
    peticio.setRawHeader("Cookie", sessio.toUtf8());

    QNetworkReply *respostaSMS = xarxa.post(peticio, xmlPeticio.toUtf8());

    QEventLoop bucle;
    connect(respostaSMS, &QNetworkReply::finished, &bucle, &QEventLoop::quit);
    QTimer::singleShot(5000, &bucle, &QEventLoop::quit);
    bucle.exec();

    QString xmlResposta = QString::fromUtf8(respostaSMS->readAll());
    respostaSMS->deleteLater();

    // Parsejar els missatges SMS del XML de resposta
    QXmlStreamReader xml(xmlResposta);
    int indexMaxim = m_ultimIndexSMS;

    struct SMSEntry {
        int     index  = -1;
        QString telefon;
        QString text;
        QString data;
    };
    QList<SMSEntry> smsList;

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == QString("Message")) {
            SMSEntry entry;
            while (!(xml.isEndElement() && xml.name() == QString("Message"))) {
                xml.readNext();
                if (!xml.isStartElement()) continue;
                if      (xml.name() == QString("Index"))   entry.index   = xml.readElementText().toInt();
                else if (xml.name() == QString("Phone"))   entry.telefon = xml.readElementText();
                else if (xml.name() == QString("Content")) entry.text    = xml.readElementText();
                else if (xml.name() == QString("Date"))    entry.data    = xml.readElementText();
            }
            if (entry.index > 0) smsList.append(entry);
        }
    }

    // Primera execució: establim l'índex màxim sense emetre signals
    if (m_primeraPollingSMS) {
        for (const SMSEntry &e : smsList)
            if (e.index > indexMaxim) indexMaxim = e.index;
        m_ultimIndexSMS    = indexMaxim;
        m_primeraPollingSMS = false;
        qDebug() << "[SMS] Índex inicial establert:" << m_ultimIndexSMS;
        return;
    }

    // Emetre signal per cada SMS nou (índex superior a l'últim processat)
    for (const SMSEntry &e : smsList) {
        if (e.index > m_ultimIndexSMS) {
            qDebug() << "[SMS] SMS nou de:" << e.telefon;
            emit smsRebut(e.telefon, e.text, e.data);
            if (e.index > indexMaxim) indexMaxim = e.index;
        }
    }
    m_ultimIndexSMS = indexMaxim;
}
