#include "equip.h"

Equip::Equip(ItemPart part) : Item(part)
{

}

QString Equip::getLevelTier() const
{
    return m_levelTier;
}

int Equip::getQuality() const
{
    return m_quality;
}

QString Equip::getSetLevel() const
{
    return m_setLevel;
}

void Equip::setLevelTier(QString levelTier)
{
    m_levelTier = levelTier;
}

void Equip::setQuality(int quality)
{
    m_quality = quality;
}

void Equip::setSetLevel(QString setLevel)
{
    m_setLevel = setLevel;
}
