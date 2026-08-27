#include "configuracio.h"
#include "ui_configuracio.h"
#include "qrwhatsapp.h"
#include "contactes.h"
#include "gestorcanals.h"
#include "serveiwhatsapp.h"
#include "adaptadorwhatsapp.h"

Configuracio::Configuracio(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Configuracio)
{
    ui->setupUi(this);
    setWindowTitle("Configuració");
}

Configuracio::~Configuracio()
{
    delete ui;
}

void Configuracio::on_btoQRWhatsapp_clicked()
{
    QRWhatsapp dlg(this);

    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (sw) {
        if (sw->adaptador()->estaDisponible()) {
            dlg.mostrarConnectat();
        } else {
            connect(sw->adaptador(), &AdaptadorWhatsApp::qrRebut,
                    &dlg, &QRWhatsapp::mostrarQr);
            connect(sw->adaptador(), &AdaptadorWhatsApp::connectat,
                    &dlg, &QRWhatsapp::mostrarConnectat);
        }
    }

    dlg.exec();
}

void Configuracio::on_btoContactes_clicked()
{
    Contactes dlg(this);
    dlg.exec();
}

void Configuracio::on_btoSortir_clicked()
{
    reject();
}
