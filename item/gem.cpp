#include "gem.h"

Gem::Gem() : Item(Part::GEM)
{

}

int Gem::GetLevel() const
{
    return mLevel;
}

QString Gem::getAttr() const
{
    return mAttr;
}

QString Gem::getEffect() const
{
    return mEffect;
}

void Gem::setLevel(int level)
{
    mLevel = level;
}

void Gem::setAttr(QString attr)
{
    mAttr = attr;
}

void Gem::setEffect(QString effect)
{
    mEffect = effect;
}
