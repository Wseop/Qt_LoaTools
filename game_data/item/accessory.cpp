#include "accessory.h"

Accessory::Accessory(ItemType type) :
    Item(type)
{

}

int Accessory::getQuality() const
{
    return m_quality;
}

const QList<QPair<Ability, int> > &Accessory::getAbilities() const
{
    return m_abilities;
}

const QList<QPair<QString, int> > &Accessory::getEngraves() const
{
    return m_engraves;
}

const QPair<QString, int> &Accessory::getPenalty() const
{
    return m_penalty;
}

void Accessory::setQuality(int quality)
{
    m_quality = quality;
}

void Accessory::addAbility(Ability ability, int value)
{
    m_abilities.append({ability, value});
}

void Accessory::addEngrave(const QString& engrave, int value)
{
    if (engrave.contains("감소"))
        setPenalty(engrave, value);
    else
        m_engraves.append({engrave, value});
}

void Accessory::setPenalty(const QString& penalty, int value)
{
    m_penalty = {penalty, value};
}
