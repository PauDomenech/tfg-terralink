#ifndef SERVEIIMPRESSIO_H
#define SERVEIIMPRESSIO_H

#include <QObject>
#include <QString>

class Missatge;
class AdaptadorImpressoraEscPos;

/**
 * @brief Servei d'impressió de missatges. Patró Strategy.
 *
 * Imprimeix missatges en paper mitjançant una impressora tèrmica
 * compatible amb el protocol ESC/POS. Gestiona el canvi de canal
 * per evitar imprimir la capçalera de canal repetidament.
 *
 * @see AdaptadorImpressoraEscPos, ControladorAplicacio
 */
class ServeiImpressio : public QObject
{
    Q_OBJECT

public:
    /** @brief Constructor. Inicialitza els membres de la classe. */
    explicit ServeiImpressio(QObject *parent = nullptr);

    /**
     * @brief Imprimeix un missatge. Gestiona automàticament la capçalera de canal.
     * @param m Missatge a imprimir.
     * @return true si s'ha imprès correctament.
     */
    bool imprimirMissatge(Missatge *m);

    /** @brief Imprimeix la capçalera inicial de l'app (TerraLink + data). */
    bool imprimirCapcalera();

private:
    AdaptadorImpressoraEscPos *m_adaptador; ///< Adaptador de la impressora ESC/POS
    QString m_ultimCanal; ///< Últim canal imprès per evitar repetir capçaleres
};

#endif // SERVEIIMPRESSIO_H
