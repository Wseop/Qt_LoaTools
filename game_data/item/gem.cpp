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

GemType Gem::getGemType() const
{
    return m_gemType;
}

void Gem::setLevel(int level)
{
    m_level = level;
}

void Gem::setEffect(const QString& effect)
{
    m_effect = effect;
}

void Gem::setGemType(GemType type)
{
    m_gemType = type;
}
