#include "repositoricontactes.h"
#include "contacte.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

RepositoriContactes::RepositoriContactes(QObject *parent)
    : QObject(parent)
{
    carregarDeFitxer();
}

RepositoriContactes& RepositoriContactes::instancia()
{
    static RepositoriContactes inst;
    return inst;
}

QList<Contacte*> RepositoriContactes::trobarTots()
{
    return m_contactes;
}

Contacte* RepositoriContactes::trobarPerId(const QString &id)
{
    for (Contacte *c : m_contactes) {
        if (c->id == id)
            return c;
    }
    return nullptr;
}

bool RepositoriContactes::desar(Contacte *c)
{
    if (!c) return false;

    for (int i = 0; i < m_contactes.size(); i++) {
        if (m_contactes[i]->numeroTelefon == c->numeroTelefon) {
            if (!c->nom.isEmpty() && c->nom != c->numeroTelefon)
                m_contactes[i]->nom = c->nom;
            guardarAFitxer();
            return true;
        }
    }

    m_contactes.append(c);
    guardarAFitxer();
    return true;
}

bool RepositoriContactes::eliminar(const QString &id)
{
    for (int i = 0; i < m_contactes.size(); i++) {
        if (m_contactes[i]->id == id) {
            m_contactes.removeAt(i);
            guardarAFitxer();
            return true;
        }
    }
    return false;
}

void RepositoriContactes::recarregar()
{
    // TODO: recarregar des de fitxer/BD
}

void RepositoriContactes::carregarDeFitxer()
{
    QFile fitxer(m_rutaFitxer);
    if (!fitxer.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(fitxer.readAll());
    fitxer.close();

    if (!doc.isArray()) return;

    for (const QJsonValue &val : doc.array()) {
        QJsonObject obj = val.toObject();
        Contacte *c = new Contacte();
        c->id            = obj["id"].toString();
        c->nom           = obj["nom"].toString();
        c->numeroTelefon = obj["numero"].toString();
        c->adrecaLora    = obj["lora"].toString();
        c->esEmergencia = obj["emergencia"].toBool(false);
        m_contactes.append(c);
    }
    qDebug() << "[Repositori] Carregats" << m_contactes.size() << "contactes";
}

void RepositoriContactes::guardarAFitxer()
{
    QJsonArray array;
    for (Contacte *c : m_contactes) {
        QJsonObject obj;
        obj["id"]     = c->id;
        obj["nom"]    = c->nom;
        obj["numero"] = c->numeroTelefon;
        obj["lora"]   = c->adrecaLora;
        obj["emergencia"] = c->esEmergencia;
        array.append(obj);
    }

    QFile fitxer(m_rutaFitxer);
    if (!fitxer.open(QIODevice::WriteOnly)) return;
    fitxer.write(QJsonDocument(array).toJson());
    fitxer.close();
    qDebug() << "[Repositori] Guardats" << m_contactes.size() << "contactes";
}

QList<Contacte*> RepositoriContactes::trobarEmergencies()
{
    QList<Contacte*> resultat;
    for (Contacte *c : m_contactes) {
        if (c->esEmergencia)
            resultat.append(c);
    }
    return resultat;
}
