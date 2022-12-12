#include "bracelet.h"

Bracelet::Bracelet() :
    Item(ItemType::팔찌)
{

}

const QStringList& Bracelet::getEffects() const
{
    return m_effects;
}

void Bracelet::addEffect(const QString& effect)
{
    m_effects << effect;
}
