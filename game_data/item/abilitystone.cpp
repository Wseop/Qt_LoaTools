#include "abilitystone.h"

AbilityStone::AbilityStone() :
    Item(ItemType::어빌리티_스톤),
    m_penalty({"", 0})
{

}

const QList<QPair<QString, int> > &AbilityStone::getEngraves() const
{
    return m_engraves;
}

const QPair<QString, int> &AbilityStone::getPenalty() const
{
    return m_penalty;
}

void AbilityStone::addEngrave(const QString& engrave, int value)
{
    m_engraves.append({engrave, value});
}

void AbilityStone::setPenalty(const QString& penalty, int value)
{
    m_penalty = {penalty, value};
}
