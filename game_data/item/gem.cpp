#include "gem.h"

Gem::Gem() :
    Item(ItemType::보석)
{

}

int Gem::getLevel() const
{
    return m_level;
}

QString Gem::getEffect() const
{
    return m_effect;
}

void Gem::setLevel(int level)
{
    m_level = level;
}

void Gem::setEffect(const QString& effect)
{
    m_effect = effect;
}
