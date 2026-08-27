#ifndef CONFIGURACIO_H
#define CONFIGURACIO_H

#include <QDialog>

namespace Ui {
class Configuracio;
}

/**
 * @brief Diàleg de configuració del sistema. Protegit per combinació de tecles.
 *
 * Accessible només mitjançant la combinació secreta (Espai, Espai, Tab, Tab).
 * Conté les opcions de gestió de contactes i connexió WhatsApp (QR).
 *
 * @see QRWhatsapp, Contactes, MainWindow
 */
class Configuracio : public QDialog
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit Configuracio(QWidget *parent = nullptr);
    ~Configuracio();

private slots:
    /** @brief Obre el diàleg de QR de WhatsApp i connecta els signals de l'adaptador. */
    void on_btoQRWhatsapp_clicked();

    /** @brief Obre el diàleg de gestió de contactes. */
    void on_btoContactes_clicked();

    /** @brief Tanca el diàleg de configuració. */
    void on_btoSortir_clicked();

private:
    Ui::Configuracio *ui;
};

#endif // CONFIGURACIO_H
