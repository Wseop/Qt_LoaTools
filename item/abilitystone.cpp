#include "abilitystone.h"

AbilityStone::AbilityStone() : Item(Part::STONE)
{

}

QString AbilityStone::getEngrave() const
{
    return mEngrave;
}

void AbilityStone::setEngrave(QString engrave)
{
    mEngrave = engrave;
}
