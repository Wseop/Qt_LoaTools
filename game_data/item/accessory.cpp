#include "accessory.h"

Accessory::Accessory(ItemPart part) : Item(part)
{

}

int Accessory::getQuality() const
{
    return m_quality;
}

QString Accessory::getAbility() const
{
    return m_ability;
}

const QStringList &Accessory::getEngraves() const
{
    return m_engraves;
}

QString Accessory::getPenalty() const
{
    return m_penalty;
}

void Accessory::setQuality(int quality)
{
    m_quality = quality;
}

void Accessory::setAbility(QString ability)
{
    m_ability = ability;
}

void Accessory::addEngrave(QString engrave)
{
    m_engraves << engrave;
}

void Accessory::setPenalty(QString penalty)
{
    m_penalty = penalty;
}
