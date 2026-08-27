#include "qrwhatsapp.h"
#include "ui_qrwhatsapp.h"

#include <QProcess>
#include <QPixmap>
#include <QDir>

QRWhatsapp::QRWhatsapp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QRWhatsapp)
{
    ui->setupUi(this);
}

QRWhatsapp::~QRWhatsapp()
{
    delete ui;
}

void QRWhatsapp::mostrarQr(const QString &asciiQr)
{
    QFont fontMono("Monospace");
    fontMono.setStyleHint(QFont::TypeWriter);
    fontMono.setPointSize(4); // petit perquè hi càpiga tot

    ui->lblQr->setFont(fontMono);
    ui->lblQr->setWordWrap(false);
    ui->lblQr->setText(asciiQr);
    ui->lblQr->adjustSize();
}

void QRWhatsapp::mostrarConnectat()
{
    QFont font;
    font.setPointSize(12);
    ui->lblQr->setFont(font);
    ui->lblQr->setWordWrap(true);
    ui->lblQr->setAlignment(Qt::AlignCenter);
    ui->lblQr->setText("WhatsApp connectat!");
}
