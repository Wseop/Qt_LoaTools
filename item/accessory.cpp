#include "accessory.h"

Accessory::Accessory(Part part) : Item(part)
{

}

int Accessory::getQuality() const
{
    return mQuality;
}

QString Accessory::getAbility() const
{
    return mAbility;
}

const QStringList &Accessory::getEngraves() const
{
    return mEngraves;
}

QString Accessory::getPenalty() const
{
    return mPenalty;
}

void Accessory::setQuality(int quality)
{
    mQuality = quality;
}

void Accessory::setAbility(QString ability)
{
    mAbility = ability;
}

void Accessory::addEngrave(QString engrave)
{
    mEngraves << engrave;
}

void Accessory::setPenalty(QString penalty)
{
    mPenalty = penalty;
}
