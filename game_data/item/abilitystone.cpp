#include "abilitystone.h"

AbilityStone::AbilityStone() : Item(ItemPart::STONE)
{

}

const QStringList &AbilityStone::getEngraves() const
{
    return m_engraves;
}

QString AbilityStone::getPenalty() const
{
    return m_penalty;
}

void AbilityStone::addEngrave(QString engrave)
{
    m_engraves << engrave;
}

void AbilityStone::setPenalty(QString penalty)
{
    m_penalty = penalty;
}
