#include "accessory.h"

Accessory::Accessory(Part part) : Item(part)
{

}

int Accessory::getQuality()
{
    return mQuality;
}

QString Accessory::getAttr()
{
    return mAttr;
}

QString Accessory::getEngrave()
{
    return mEngrave;
}

void Accessory::setQuality(int quality)
{
    mQuality = quality;
}

void Accessory::setAttr(QString attr)
{
    mAttr = attr;
}

void Accessory::setEngrave(QString engrave)
{
    mEngrave = engrave;
}
