#include "equip.h"

Equip::Equip(ItemType type) : Item(type)
{

}

int Equip::getReforge()
{
    return m_reforge;
}

QString Equip::getLevelTier() const
{
    return m_levelTier;
}

int Equip::getQuality() const
{
    return m_quality;
}

SetEffect Equip::getSetEffect() const
{
    return m_setEffect;
}

QString Equip::getSetLevel() const
{
    return m_setLevel;
}

void Equip::setReforge(int reforge)
{
    m_reforge = reforge;
}

void Equip::setLevelTier(QString levelTier)
{
    m_levelTier = levelTier;
}

void Equip::setQuality(int quality)
{
    m_quality = quality;
}

void Equip::setSetEffect(SetEffect setEffect)
{
    m_setEffect = setEffect;
}

void Equip::setSetLevel(QString setLevel)
{
    m_setLevel = setLevel;
}
