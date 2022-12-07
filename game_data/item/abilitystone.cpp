#include "abilitystone.h"

AbilityStone::AbilityStone() :
    Item(ItemType::어빌리티_스톤)
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

void AbilityStone::addEngrave(QString engrave, int value)
{
    m_engraves.append({engrave, value});
}

void AbilityStone::setPenalty(QString penalty, int value)
{
    m_penalty = {penalty, value};
}
