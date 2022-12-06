#include "bracelet.h"

Bracelet::Bracelet() : Item(ItemPart::BRACELET)
{

}

QString Bracelet::getEffect() const
{
    return m_effect;
}

void Bracelet::setEffect(QString effect)
{
    m_effect = effect;
}
