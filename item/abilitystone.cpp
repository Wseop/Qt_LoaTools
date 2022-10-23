#include "abilitystone.h"

AbilityStone::AbilityStone() : Item(Part::STONE)
{

}

QString AbilityStone::getEngrave()
{
    return mEngrave;
}

void AbilityStone::setEngrave(QString engrave)
{
    mEngrave = engrave;
}
