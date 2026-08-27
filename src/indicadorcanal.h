#ifndef INDICADORCANAL_H
#define INDICADORCANAL_H

#include <QWidget>
#include <QLabel>
#include <QString>

/**
 * @brief Estat de disponibilitat d'un canal de comunicació.
 */
enum class EstatCanal {
    Desconegut,   ///< Encara no s'ha comprovat l'estat del canal
    Connectat,    ///< El canal respon i està disponible
    Desconnectat  ///< El canal no respon o no està disponible
};

/**
 * @brief Widget d'indicador visual (etiqueta + LED) per a un canal de comunicació.
 *
 * Mostra el nom curt del canal (WA, SMS, LoRa) junt amb un indicador
 * lluminós de color (verd/vermell/gris) que reflecteix la seva disponibilitat
 * actual. Implementa el requisit RF8. Pensat per usar-se com a widget
 * promogut ("Promote to...") des del Qt Designer: el constructor només
 * accepta el parent, i el text es configura després amb setEtiqueta().
 *
 * @see ServeiMonitoratge, EstatSistema, MainWindow
 */
class IndicadorCanal : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor. Crea el widget buit, amb el LED en estat Desconegut.
     * @param parent Widget pare.
     */
    explicit IndicadorCanal(QWidget *parent = nullptr);

    /**
     * @brief Estableix el text identificatiu del canal.
     * @param etiqueta Text curt (ex: "WA", "SMS", "LoRa").
     */
    void setEtiqueta(const QString &etiqueta);

    /**
     * @brief Actualitza el color del LED segons l'estat del canal.
     * @param estat Nou estat a representar (Connectat / Desconnectat / Desconegut).
     */
    void setEstat(EstatCanal estat);

private:
    QLabel *m_etiqueta; ///< Text identificatiu del canal (ex: "WA")
    QLabel *m_led;      ///< Punt de color que representa l'estat de connexió
};

#endif // INDICADORCANAL_H
