#include "indicadorcanal.h"

#include <QHBoxLayout>
#include <QFont>

IndicadorCanal::IndicadorCanal(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(6, 2, 6, 2);
    layout->setSpacing(5);

    m_etiqueta = new QLabel(this);
    QFont font = m_etiqueta->font();
    font.setPointSize(11);
    font.setBold(true);
    m_etiqueta->setFont(font);

    m_led = new QLabel(this);
    m_led->setFixedSize(12, 12);

    layout->addWidget(m_etiqueta);
    layout->addWidget(m_led);

    setEstat(EstatCanal::Desconegut);
}

void IndicadorCanal::setEtiqueta(const QString &etiqueta)
{
    m_etiqueta->setText(etiqueta);
}

void IndicadorCanal::setEstat(EstatCanal estat)
{
    QString color;
    switch (estat) {
    case EstatCanal::Connectat:    color = "#2ecc71"; break; // verd
    case EstatCanal::Desconnectat: color = "#e74c3c"; break; // vermell
    default:                       color = "#95a5a6"; break; // gris
    }
    m_led->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(color));
}
