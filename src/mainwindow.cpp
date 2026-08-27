#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "contactes.h"
#include "enviarmissatge.h"
#include "missatgesrebuts.h"
#include "qrwhatsapp.h"
#include "configuracio.h"
#include "controladoraplicacio.h"
#include "repositoricontactes.h"
#include "contacte.h"
#include "gestorcanals.h"
#include "serveiwhatsapp.h"
#include "adaptadorwhatsapp.h"
#include "iserveimissatgeria.h"
#include "serveimonitoratge.h"
#include "estatsistema.h"

#include <QMessageBox>
#include <QShortcut>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setStyleSheet(
        "QPushButton:focus {"
        "border: 3px solid orange;"
        "background-color: lightyellow;"
        "}"
        );

    ui->setupUi(this);

    // Inicialitzar el controlador (connecta WhatsApp, SMS, impressora, etc.)
    ControladorAplicacio::instancia().inicialitzar();
    // RF8 — indicadors d'estat dels canals de comunicació (icona + LED)
    ui->ledWhatsapp->setEtiqueta("WA");
    ui->ledSms->setEtiqueta("SMS");
    ui->ledLora->setEtiqueta("LoRa");

    connect(&ServeiMonitoratge::instancia(), &ServeiMonitoratge::estatCanviat,
            this, [this](EstatSistema *estat) {
                ui->ledWhatsapp->setEstat(estat->whatsappDisponible ? EstatCanal::Connectat : EstatCanal::Desconnectat);
                ui->ledSms->setEstat(estat->smsDisponible ? EstatCanal::Connectat : EstatCanal::Desconnectat);
                ui->ledLora->setEstat(estat->loraDisponible ? EstatCanal::Connectat : EstatCanal::Desconnectat);
            });

    ServeiMonitoratge::instancia().iniciarMonitoratge();

    QShortcut *ctrlJ = new QShortcut(QKeySequence("Ctrl+J"), this);
    ctrlJ->setContext(Qt::ApplicationShortcut);
    connect(ctrlJ, &QShortcut::activated, this, &MainWindow::onEmergencia);

    // Connectar botons principals
    connect(ui->btoEnvMiss,    &QPushButton::clicked, this, &MainWindow::onEnviarMissatge);
    connect(ui->btoMissRebuts, &QPushButton::clicked, this, &MainWindow::onMissatgesRebuts);
    connect(ui->btoEmerg,      &QPushButton::clicked, this, &MainWindow::onEmergencia);

    // Instal·lar filtre d'events per a la combinació secreta de configuració
    qApp->installEventFilter(this);

    //Posar focus al botó missatges rebuts
    ui->btoMissRebuts->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->isAutoRepeat())
            return QMainWindow::eventFilter(obj, event);

        int tecla = keyEvent->key();

        // Filtre de duplicats (debounce): ignora la mateixa tecla si es repeteix
        // en menys de 150ms (típic d'alguns teclats USB que envien múltiples
        // informes HID per una sola pulsació física)
        if (tecla == m_ultimaTecla && m_debounceTimer.isValid() && m_debounceTimer.elapsed() < 150) {
            return QMainWindow::eventFilter(obj, event);
        }
        m_ultimaTecla = tecla;
        m_debounceTimer.restart();

        if (tecla == Qt::Key_1 || tecla == Qt::Key_2 || tecla == Qt::Key_3 || tecla == Qt::Key_4) {
            m_combinacioActual.append(tecla);

            while (m_combinacioActual.size() > m_combinacioSecreta.size())
                m_combinacioActual.removeFirst();

            if (m_combinacioActual == m_combinacioSecreta) {
                m_combinacioActual.clear();
                onConfiguracio();
            }
        }
        else if (tecla != Qt::Key_Tab && tecla != Qt::Key_Backtab) {
            m_combinacioActual.clear();
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onEnviarMissatge()
{
    EnviarMissatge dlg(this);
    dlg.exec();
}

void MainWindow::onMissatgesRebuts()
{
    MissatgesRebuts dlg(this);
    dlg.exec();
}

void MainWindow::onContactes()
{
    Contactes dlg(this);
    dlg.exec();
}

void MainWindow::onQRWhatsapp()
{
    QRWhatsapp dlg(this);

    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (sw) {
        connect(sw->adaptador(), &AdaptadorWhatsApp::qrRebut,    &dlg, &QRWhatsapp::mostrarQr);
        connect(sw->adaptador(), &AdaptadorWhatsApp::connectat,  &dlg, &QRWhatsapp::mostrarConnectat);
    }

    dlg.exec();
}

void MainWindow::onEmergencia()
{
    /* if (QMessageBox::question(this, "Emergència",
                              "Vols enviar un missatge d'emergència?",
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No) == QMessageBox::Yes)
    {*/
        QList<Contacte*> contactes = RepositoriContactes::instancia().trobarTots();
        if (contactes.isEmpty()) {
            QMessageBox::warning(this, "Emergència", "No hi ha contactes configurats.");
            return;
        }
        ControladorAplicacio::instancia().activarEmergencia(contactes.first());
    //}
}

void MainWindow::onConfiguracio()
{
    Configuracio dlg(this);
    dlg.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Aturar el procés WhatsApp en tancar l'aplicació
    ServeiWhatsApp *sw = qobject_cast<ServeiWhatsApp*>(GestorCanals::obtenir("whatsapp"));
    if (sw)
        sw->aturarConnexio();
    event->accept();
}
