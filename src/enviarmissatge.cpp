#include "enviarmissatge.h"
#include "ui_enviarmissatge.h"
#include "controladoraplicacio.h"
#include "repositoricontactes.h"
#include "contacte.h"

#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QMetaObject>

EnviarMissatge::EnviarMissatge(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EnviarMissatge)
    , m_gravant(false)
    , m_indexContacte(0)
{
    ui->setupUi(this);
    ui->btoGravar->setText("🎤 Gravar");
    ui->lblEstat->setText("Selecciona un contacte i prem Gravar");
    ui->btoEnviar->setEnabled(false);
    ui->txtTranscripcio->setReadOnly(false);
    ui->txtTranscripcio->setPlaceholderText("El text transcrit apareixerà aquí...");
    carregarContactes();
}

EnviarMissatge::~EnviarMissatge()
{
    delete ui;
}

void EnviarMissatge::carregarContactes()
{
    m_contactes = RepositoriContactes::instancia().trobarTots();
    m_indexContacte = 0;
    actualitzarLabelContacte();
}

void EnviarMissatge::actualitzarLabelContacte()
{
    if (m_contactes.isEmpty()) {
        ui->lblContacte->setText("No hi ha contactes disponibles");
        return;
    }
    Contacte *c = m_contactes.at(m_indexContacte);
    ui->lblContacte->setText(c->nom + "\n" + c->numeroTelefon);
}

void EnviarMissatge::on_btoCiclarContacte_clicked()
{
    if (m_contactes.isEmpty()) return;
    m_indexContacte = (m_indexContacte + 1) % m_contactes.size();
    actualitzarLabelContacte();
}

Contacte* EnviarMissatge::contacteSeleccionat()
{
    if (m_contactes.isEmpty()) return nullptr;
    return m_contactes.at(m_indexContacte);
}

void EnviarMissatge::on_btoGravar_clicked()
{
    if (!m_gravant) {
        // Iniciar gravació
        if (!contacteSeleccionat()) {
            QMessageBox::warning(this, "Error", "No hi ha contactes disponibles.");
            return;
        }

        bool ok = ControladorAplicacio::instancia().iniciarGravacio();
        if (!ok) {
            QMessageBox::warning(this, "Error",
                                 "No s'ha pogut iniciar la gravació.\nComprova el micròfon.");
            return;
        }

        m_gravant = true;
        ui->btoGravar->setText("Aturar");
        ui->lblEstat->setText("Gravant...");
        ui->btoEnviar->setEnabled(false);
        ui->txtTranscripcio->clear();

    } else {
        // Aturar gravació
        m_gravant = false;
        ui->btoGravar->setEnabled(false);
        ui->lblEstat->setText("Transcrivint... (pot trigar uns segons)");

        QString rutaAudio = ControladorAplicacio::instancia().aturarITranscriure();

        // Llançar transcripció de forma asíncrona
        m_procesTranscripcio = new QProcess(this);
        connect(m_procesTranscripcio,
                QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &EnviarMissatge::onTranscripcioAcabada);

        m_procesTranscripcio->start("python3", {
            "/home/paudomenech/TerraLink/transcriure.py",
            rutaAudio
        });
    }
}

void EnviarMissatge::on_btoEnviar_clicked()
{
    QString text = ui->txtTranscripcio->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Error", "El text està buit.");
        return;
    }

    Contacte *c = contacteSeleccionat();
    if (!c) {
        QMessageBox::warning(this, "Error", "Selecciona un contacte.");
        return;
    }

    ControladorAplicacio::instancia().enviarTextAContacte(text, c);

    ui->lblEstat->setText("Missatge enviat!");
    QMessageBox::information(this, "Enviat", "Missatge enviat correctament.");
    accept();
}

void EnviarMissatge::onTranscripcioAcabada(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)

    QString text = QString::fromUtf8(m_procesTranscripcio->readAllStandardOutput()).trimmed();
    m_procesTranscripcio->deleteLater();
    m_procesTranscripcio = nullptr;

    if (exitCode != 0 || text.isEmpty()) {
        ui->lblEstat->setText("ssNo s'ha pogut transcriure. Torna a provar.");
        ui->btoGravar->setText("Gravar");
        ui->btoGravar->setEnabled(true);
        return;
    }

    ui->txtTranscripcio->setPlainText(text);
    ui->lblEstat->setText("Revisa el text i prem Enviar");
    ui->btoGravar->setText("Tornar a gravar");
    ui->btoGravar->setEnabled(true);
    ui->btoEnviar->setEnabled(true);
}

void EnviarMissatge::on_btoSortir_clicked()
{
    reject();
}
