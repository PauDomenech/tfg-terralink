#ifndef ENVIARMISSATGE_H
#define ENVIARMISSATGE_H

#include <QDialog>
#include <QList>
#include <QProcess>

namespace Ui {
class EnviarMissatge;
}

class Contacte;

/**
 * @brief Diàleg per gravar, transcriure i enviar missatges de veu.
 *
 * Flux d'ús: Seleccionar contacte → Gravar → (transcripció async) → Revisar text → Enviar.
 * La transcripció es fa de forma asíncrona via QProcess (faster-whisper)
 * per no bloquejar la UI durant el procés.
 *
 * @see ControladorAplicacio, ServeiAudio, ServeiTranscripcio
 */
class EnviarMissatge : public QDialog
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit EnviarMissatge(QWidget *parent = nullptr);
    ~EnviarMissatge();

private slots:
    /** @brief Alterna entre iniciar gravació i aturar+llançar transcripció asíncrona. */
    void on_btoGravar_clicked();

    /** @brief Envia el text transcrit (o editat) al contacte seleccionat per WhatsApp. */
    void on_btoEnviar_clicked();

    /** @brief Cicla al següent contacte de la llista (navegació amb 3 botons físics). */
    void on_btoCiclarContacte_clicked();

    /** @brief Tanca el diàleg. */
    void on_btoSortir_clicked();

    /**
     * @brief Slot cridat quan el procés de transcripció (python3) acaba.
     * @param exitCode Codi de sortida del procés (0 = OK).
     * @param exitStatus Estat de sortida del procés.
     */
    void onTranscripcioAcabada(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::EnviarMissatge *ui;

    bool             m_gravant = false;           ///< true si s'està gravant àudio
    int              m_indexContacte = 0;         ///< Índex del contacte seleccionat
    QList<Contacte*> m_contactes;                 ///< Llista de contactes del repositori
    QProcess        *m_procesTranscripcio = nullptr; ///< Procés async de transcripció Whisper

    /** @brief Carrega els contactes del RepositoriContactes a m_contactes. */
    void carregarContactes();

    /** @brief Actualitza el label de contacte amb el nom i número del contacte actual. */
    void actualitzarLabelContacte();

    /**
     * @brief Retorna el contacte seleccionat actualment.
     * @return Punter al contacte, o nullptr si la llista és buida.
     */
    Contacte* contacteSeleccionat();
};

#endif // ENVIARMISSATGE_H
