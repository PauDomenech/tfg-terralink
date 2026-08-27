#ifndef ADAPTADORIMPRESSORAESCPOS_H
#define ADAPTADORIMPRESSORAESCPOS_H

#include <QObject>
#include <QString>
#include <QDateTime>

/**
 * @brief Adaptador per a la impressora tèrmica ESC/POS via USB. Patró Adapter.
 *
 * Gestiona la comunicació amb la impressora tèrmica escrivint
 * comandes ESC/POS directament al port USB (/dev/usb/lp0).
 * Suporta text en negreta, capçaleres de canal i impressió d'imatges
 * en format ràster (GS v 0).
 *
 * @see ServeiImpressio
 */
class AdaptadorImpressoraEscPos : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit AdaptadorImpressoraEscPos(QObject *parent = nullptr);

    /**
     * @brief Imprimeix nom i text en format simple (mètode llegat).
     * @param nom Nom del remitent.
     * @param text Cos del missatge.
     * @return false sempre (no implementat, usar imprimirMissatgeFormat).
     */
    bool imprimir(const QString &nom, const QString &text);

    /**
     * @brief Comprova si la impressora respon al port USB.
     * @return true si /dev/usb/lp0 existeix i és accessible.
     */
    bool estaDisponible();

    /**
     * @brief Imprimeix la capçalera inicial de l'aplicació.
     *
     * Escriu "--- TerraLink ---" en negreta i la data/hora actual.
     * S'executa una sola vegada a l'inici via ServeiImpressio.
     * @return true si s'ha escrit correctament al port.
     */
    bool imprimirCapcalera();

    /**
     * @brief Imprimeix un missatge amb format de canal i text.
     * @param canal Nom del canal ("whatsapp", "sms", "lora") — s'imprimeix en majúscules si mostrarCanal és true.
     * @param hora  Data i hora del missatge (no s'usa actualment, reservat per ús futur).
     * @param text  Cos del missatge a imprimir.
     * @param mostrarCanal true si s'ha de mostrar la capçalera del canal (quan canvia de canal).
     * @return true si s'ha escrit correctament al port.
     */
    bool imprimirMissatgeFormat(const QString &canal,
                                const QDateTime &hora,
                                const QString &text,
                                bool mostrarCanal);

    /**
     * @brief Imprimeix una imatge a partir del seu contingut en base64.
     * @param base64 Dades de la imatge codificades en base64 (JPEG o PNG).
     * @return true si s'ha imprès correctament.
     */
    bool imprimirImatge(const QString &base64);

    /**
     * @brief Imprimeix una imatge a partir de la ruta al fitxer.
     * @param ruta Ruta absoluta al fitxer d'imatge (JPEG o PNG).
     * @return true si s'ha imprès correctament.
     */
    bool imprimirImatgeDesDeRuta(const QString &ruta);

private:
    QString m_port; ///< Port USB de la impressora (ex: "/dev/usb/lp0")

    /**
     * @brief Escriu dades raw al port USB de la impressora.
     * @param dades Bytes a enviar (comandes ESC/POS + text codificat).
     * @return true si s'ha obert el port i escrit correctament.
     */
    bool escriureAlPort(const QByteArray &dades);

    static const QByteArray CMD_INIT;        ///< ESC @ — Inicialitzar impressora
    static const QByteArray CMD_NEGRETA_ON;  ///< ESC E 1 — Activar negreta
    static const QByteArray CMD_NEGRETA_OFF; ///< ESC E 0 — Desactivar negreta
    static const QByteArray CMD_TALL;        ///< GS V B 0 — Tall parcial de paper
    static const QByteArray CMD_ALIMENTAR;   ///< ESC d 2 — Alimentar 2 línies
};

#endif // ADAPTADORIMPRESSORAESCPOS_H
