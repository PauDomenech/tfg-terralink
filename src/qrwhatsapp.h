#ifndef QRWHATSAPP_H
#define QRWHATSAPP_H

#include <QDialog>

namespace Ui {
class QRWhatsapp;
}

/**
 * @brief Diàleg per mostrar el codi QR d'autenticació de WhatsApp.
 *
 * Mostra el QR en format ASCII art (font monospace, mida 4pt) mentre
 * WhatsApp no està autenticat. Un cop autenticat, mostra el missatge
 * de connexió satisfactòria.
 *
 * @see ServeiWhatsApp, AdaptadorWhatsApp, Configuracio
 */
class QRWhatsapp : public QDialog
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit QRWhatsapp(QWidget *parent = nullptr);
    ~QRWhatsapp();

    /**
     * @brief Mostra el QR en format ASCII art al diàleg.
     * @param dadesQr Text ASCII art del codi QR.
     */
    void mostrarQr(const QString &dadesQr);

    /** @brief Substitueix el QR per un missatge de connexió satisfactòria. */
    void mostrarConnectat();

private:
    Ui::QRWhatsapp *ui;
};

#endif // QRWHATSAPP_H
