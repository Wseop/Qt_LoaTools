#include "equip.h"

Equip::Equip(Part part) : Item(part)
{

}

QString Equip::getLevelTier()
{
    return mLevelTier;
}

int Equip::getQuality()
{
    return mQuality;
}

QString Equip::getSetLevel()
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
