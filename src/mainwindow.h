#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QElapsedTimer>
#include "indicadorcanal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Finestra principal de l'aplicació TerraLink.
 *
 * Gestiona el menú principal amb 3 botons visibles (Enviar Missatge,
 * Missatges Rebuts, Emergència). La navegació es fa via Tab/Shift+Tab
 * i Espai per facilitar l'ús a persones grans amb mobilitat reduïda.
 *
 * La pantalla de Configuració és accessible via la combinació secreta:
 * Espai → Espai → Tab → Tab.
 *
 * @see ControladorAplicacio, EnviarMissatge, MissatgesRebuts, Configuracio
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief Atura el procés Node.js de WhatsApp en tancar la finestra.
     * @param event Evento de tancament de la finestra.
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief Intercepta pulsacions de tecla per detectar la combinació secreta de configuració.
     * Combinació: Espai, Espai, Tab, Tab → obre Configuracio.
     * @param obj Objecte que ha rebut l'event.
     * @param event Event capturat.
     * @return false sempre (propaga l'event per no bloquejar Tab/Shift+Tab).
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    /** @brief Obre el diàleg EnviarMissatge. */
    void onEnviarMissatge();

    /** @brief Obre el diàleg MissatgesRebuts. */
    void onMissatgesRebuts();

    /** @brief Obre el diàleg Contactes (accés intern, no via botó principal). */
    void onContactes();

    /** @brief Obre el diàleg QRWhatsapp (accés intern). */
    void onQRWhatsapp();

    /** @brief Mostra confirmació i activa el protocol d'emergència. */
    void onEmergencia();

    /** @brief Obre el diàleg Configuracio (activat per combinació secreta). */
    void onConfiguracio();

private:
    Ui::MainWindow *ui;
    QList<int> m_combinacioSecreta = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4};
    QList<int> m_combinacioActual; ///< Tecles premudes fins ara (finestra lliscant)
    QElapsedTimer m_debounceTimer; ///< Cronòmetre per filtrar pulsacions duplicades (debounce) generades per alguns teclats USB
    int m_ultimaTecla = -1;        ///< Codi de l'última tecla processada, usat pel filtre de debounce
    IndicadorCanal *m_ledWhatsapp; ///< Indicador d'estat del canal WhatsApp (RF8)
    IndicadorCanal *m_ledSms;      ///< Indicador d'estat del canal SMS (RF8)
    IndicadorCanal *m_ledLora;     ///< Indicador d'estat del canal LoRa (RF8)
};

#endif // MAINWINDOW_H
