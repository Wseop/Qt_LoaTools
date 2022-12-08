#include "bracelet.h"

Bracelet::Bracelet() :
    Item(ItemType::팔찌)
{

}

QString Bracelet::getEffect() const
{
    return m_effect;
}

void Bracelet::setEffect(const QString& effect)
{
    m_effect = effect;
}
