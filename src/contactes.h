#ifndef CONTACTES_H
#define CONTACTES_H

#include <QDialog>

namespace Ui {
class Contactes;
}

/**
 * @brief Diàleg de gestió de contactes.
 *
 * Permet afegir, modificar, eliminar i importar contactes des de WhatsApp.
 * Els contactes importats es filtren per longitud de número (10-12 dígits)
 * i es desen al RepositoriContactes per persistència.
 *
 * @see RepositoriContactes, ServeiWhatsApp, AdaptadorWhatsApp
 */
class Contactes : public QDialog
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit Contactes(QWidget *parent = nullptr);
    ~Contactes();

private slots:
    /** @brief Afegeix un nou contacte manualment. */
    void on_btoAfegir_clicked();

    /** @brief Elimina el contacte seleccionat. */
    void on_btoEliminar_clicked();

    /** @brief Modifica el contacte seleccionat. */
    void on_btoModificar_clicked();

    /** @brief Tanca el diàleg. */
    void on_btoSortir_clicked();

    /** @brief Demana els contactes a WhatsApp via AdaptadorWhatsApp. */
    void on_btoImportarWA_clicked();

    /**
     * @brief Rep i processa el JSON de contactes de WhatsApp.
     * @param json JSON amb la llista de contactes de WhatsApp.
     */
    void onContactesRebuts(const QString &json);

    /** @brief Marca o desmarca el contacte seleccionat com a contacte d'emergència. */
    void on_btoMarcarEmergencia_clicked();

private:
    Ui::Contactes *ui;
};

#endif // CONTACTES_H
