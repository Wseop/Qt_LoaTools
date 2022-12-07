#include "gem.h"

Gem::Gem() :
    Item(ItemType::보석)
{

}

int Gem::GetLevel() const
{
    return m_level;
}

QString Gem::getAttr() const
{
    return m_attr;
}

QString Gem::getEffect() const
{
    return m_effect;
}

void Gem::setLevel(int level)
{
    m_level = level;
}

void Gem::setAttr(QString attr)
{
    m_attr = attr;
}

void Gem::setEffect(QString effect)
{
    m_effect = effect;
}
