#include "equip.h"

Equip::Equip(Part part) : Item(part)
{

}

QString Equip::getLevelTier() const
{
    return mLevelTier;
}

int Equip::getQuality() const
{
    return mQuality;
}

QString Equip::getSetLevel() const
{
    return mSetLevel;
}

void Equip::setLevelTier(QString levelTier)
{
    mLevelTier = levelTier;
}

void Equip::setQuality(int quality)
{
    mQuality = quality;
}

void Equip::setSetLevel(QString setLevel)
{
    mSetLevel = setLevel;
}
