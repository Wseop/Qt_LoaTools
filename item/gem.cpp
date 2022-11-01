#include "gem.h"

Gem::Gem() : Item(Part::GEM)
{

}

QString Gem::GetLevel() const
{
    return mLevel;
}

QString Gem::getAttr() const
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
