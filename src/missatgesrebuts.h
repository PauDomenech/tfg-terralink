#ifndef MISSATGESREBUTS_H
#define MISSATGESREBUTS_H

#include <QDialog>

namespace Ui {
class MissatgesRebuts;
}

class Missatge;

/**
 * @brief Diàleg per visualitzar i imprimir els missatges rebuts.
 *
 * Mostra els últims 20 missatges del RepositoriMissatges (WhatsApp, SMS i LoRa)
 * i permet imprimir-ne qualsevol via la impressora tèrmica ESC/POS.
 * El botó "Prova foto" permet verificar la impressió d'imatges.
 *
 * @see RepositoriMissatges, ServeiImpressio, ControladorAplicacio
 */
class MissatgesRebuts : public QDialog
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit MissatgesRebuts(QWidget *parent = nullptr);
    ~MissatgesRebuts();

private slots:
    /** @brief Imprimeix el missatge seleccionat via ServeiImpressio. */
    void on_btoImprimir_clicked();

    /** @brief Tanca el diàleg. */
    void on_btoTancar_clicked();

    /** @brief Imprimeix una foto de prova des de /home/paudomenech/TerraLink/prova.jpg. */
    void on_btoProvaFoto_clicked();

private:
    Ui::MissatgesRebuts *ui;

    /** @brief Carrega els missatges recents del repositori i els mostra a la llista. */
    void carregarMissatges();

    QList<Missatge*> m_missatges; ///< Missatges carregats del repositori (últims 20)
};

#endif // MISSATGESREBUTS_H
