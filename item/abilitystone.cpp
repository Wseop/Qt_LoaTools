#include "abilitystone.h"

AbilityStone::AbilityStone() : Item(Part::STONE)
{

}

const QStringList &AbilityStone::getEngraves() const
{
    return mEngraves;
}

QString AbilityStone::getPenalty() const
{
    return mPenalty;
}

void AbilityStone::addEngrave(QString engrave)
{
    mEngraves << engrave;
}

void AbilityStone::setPenalty(QString penalty)
{
    mPenalty = penalty;
}
