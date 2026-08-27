#include "contactes.h"
#include "ui_contactes.h"
#include "gestorcanals.h"
#include "serveiwhatsapp.h"
#include "adaptadorwhatsapp.h"
#include "repositoricontactes.h"
#include "contacte.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <algorithm>

static bool compararContactes(Contacte *a, Contacte *b)
{
    return a->nom.toLower() < b->nom.toLower();
}

Contactes::Contactes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Contactes)
{
    ui->setupUi(this);
    ui->btoImportarWA->setFocus();

    // Carregar i ordenar contactes alfabèticament
    QList<Contacte*> contactes = RepositoriContactes::instancia().trobarTots();
    std::sort(contactes.begin(), contactes.end(), compararContactes);

    for (Contacte *c : contactes)
        ui->listWidget->addItem(c->nom + " - " + c->numeroTelefon);
}

Contactes::~Contactes()
{
    delete ui;
}

void Contactes::on_btoAfegir_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Afegir contacte",
                                         "Nom i telèfon:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
        ui->listWidget->addItem(text);
}

void Contactes::on_btoEliminar_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Eliminar contacte", "Selecciona un contacte.");
        return;
    }
    if (QMessageBox::question(this, "Confirmar eliminació",
                              "Segur que vols eliminar el contacte seleccionat?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) == QMessageBox::Yes)
        delete item;
}

void Contactes::on_btoModificar_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Modificar contacte", "Selecciona un contacte.");
        return;
    }
    bool ok;
    QString text = QInputDialog::getText(this, "Modificar contacte",
                                         "Edita el contacte:", QLineEdit::Normal,
                                         item->text(), &ok);
    if (ok && !text.isEmpty())
        item->setText(text);
}

void Contactes::on_btoSortir_clicked()
{
    reject();
}

void Contactes::on_btoImportarWA_clicked()
{
    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (!sw || !sw->adaptador()->estaDisponible()) {
        QMessageBox::warning(this, "Importar contactes",
                             "WhatsApp no està connectat. Obre primer el QR.");
        return;
    }

    connect(sw->adaptador(), &AdaptadorWhatsApp::contactesRebuts,
            this, &Contactes::onContactesRebuts);

    sw->obtenirContactes();
}

void Contactes::onContactesRebuts(const QString &json)
{
    qDebug() << "onContactesRebuts cridat, mida json:" << json.size();

    // Desconnectar per evitar dobles activacions
    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (sw)
        disconnect(sw->adaptador(), &AdaptadorWhatsApp::contactesRebuts,
                   this, &Contactes::onContactesRebuts);

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        QMessageBox::warning(this, "Importar contactes", "Error parsejant els contactes.");
        return;
    }

    // Filtrar i desar al repositori
    for (const QJsonValue &val : doc.array()) {
        QJsonObject obj = val.toObject();
        QString numero = obj["number"].toString();
        QString nom    = obj["name"].toString();

        // Filtres: número vàlid, nom diferent del número, longitud 10-12 dígits
        if (numero.isEmpty() || nom.isEmpty() || nom == numero) continue;
        if (numero.length() < 10 || numero.length() > 12) continue;

        Contacte *c = new Contacte();
        c->nom           = nom;
        c->numeroTelefon = numero;
        RepositoriContactes::instancia().desar(c);
    }

    // Refrescar llista ordenada
    ui->listWidget->clear();
    QList<Contacte*> contactes = RepositoriContactes::instancia().trobarTots();
    std::sort(contactes.begin(), contactes.end(), compararContactes);

    for (Contacte *c : contactes)
        ui->listWidget->addItem(c->nom + " - " + c->numeroTelefon);

    QMessageBox::information(this, "Importar contactes",
                             QString("S'han importat %1 contactes.").arg(contactes.size()));
}

void Contactes::on_btoMarcarEmergencia_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Marcar emergència", "Selecciona un contacte.");
        return;
    }

    // Obtenir l'índex seleccionat per trobar el contacte real corresponent
    int index = ui->listWidget->currentRow();
    QList<Contacte*> contactes = RepositoriContactes::instancia().trobarTots();
    std::sort(contactes.begin(), contactes.end(), compararContactes);

    if (index < 0 || index >= contactes.size()) return;

    Contacte *c = contactes[index];
    c->esEmergencia = !c->esEmergencia; // alternar
    RepositoriContactes::instancia().desar(c); // ja fa persistència al JSON

    QString estat = c->esEmergencia ? "marcat com a emergència" : "desmarcat com a emergència";
    QMessageBox::information(this, "Contacte actualitzat",
                             QString("%1 %2.").arg(c->nom, estat));

    // Refrescar la llista per mostrar l'estat (opcional: afegir icona/asterisc)
    ui->listWidget->clear();
    for (Contacte *ct : contactes) {
        QString etiqueta = ct->nom + " - " + ct->numeroTelefon;
        if (ct->esEmergencia) etiqueta += " ⚠";
        ui->listWidget->addItem(etiqueta);
    }
}
