#include "gem.h"

Gem::Gem() : Item(Part::GEM)
{

}

QString Gem::GetLevel()
{
    return mLevel;
}

QString Gem::getAttr()
{
    return mAttr;
}

void Gem::setLevel(QString level)
{
    mLevel = level;
}

void Gem::setAttr(QString attr)
{
    mAttr = attr;
}
