#include "missatgesrebuts.h"
#include "ui_missatgesrebuts.h"
#include "repositorimissatges.h"
#include "missatge.h"
#include "serveiimpressio.h"
#include "missatgewhatsapp.h"

#include <QMessageBox>
#include <QDebug>

MissatgesRebuts::MissatgesRebuts(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MissatgesRebuts)
{
    ui->setupUi(this);
    carregarMissatges();
}

MissatgesRebuts::~MissatgesRebuts()
{
    delete ui;
}

void MissatgesRebuts::carregarMissatges()
{
    ui->listMissatges->clear();
    m_missatges = RepositoriMissatges::instancia().trobarRecents();

    if (m_missatges.isEmpty()) {
        ui->listMissatges->addItem("No hi ha missatges recents.");
        return;
    }

    for (Missatge *m : m_missatges) {
        QString linia = QString("[%1] %2: %3")
        .arg(m->dataCreacio.toString("dd/MM hh:mm"))
            .arg(m->canal.toUpper())
            .arg(m->text);
        ui->listMissatges->addItem(linia);
    }
}

void MissatgesRebuts::on_btoImprimir_clicked()
{
    int index = ui->listMissatges->currentRow();

    if (index < 0 || index >= m_missatges.size()) {
        QMessageBox::warning(this, "Imprimir",
                             "Selecciona un missatge per imprimir.");
        return;
    }

    Missatge *m = m_missatges.at(index);
    ServeiImpressio servei;

    bool ok = servei.imprimirMissatge(m);

    if (ok) {
        QMessageBox::information(this, "Imprimir",
                                 "Missatge imprès correctament.");
    } else {
        QMessageBox::warning(this, "Imprimir",
                             "No s'ha pogut imprimir. Comprova la impressora.");
    }
}

void MissatgesRebuts::on_btoTancar_clicked()
{
    reject();
}

void MissatgesRebuts::on_btoProvaFoto_clicked()
{
    // Crea un missatge de prova amb una foto
    MissatgeWhatsApp *m = new MissatgeWhatsApp();
    m->text    = "21:47 - Prova: [FOTO]";
    m->canal   = "whatsapp";
    m->rutaAudio = "/home/paudomenech/TerraLink/prova.jpg"; // ← posa aquí la ruta d'una foto que tinguis
    m->marcarRebut();

    ServeiImpressio servei;
    servei.imprimirMissatge(m);
}
