#include "skill.h"

Skill::Skill()
{

}

QString Skill::getName() const
{
    return mName;
}

QString Skill::getIconPath() const
{
    return mIconPath;
}

int Skill::getLevel() const
{
    return mLevel;
}

QList<Tripod> Skill::getTripods() const
{
    return mTripods;
}

Tripod Skill::getTripod(int index) const
{
    if (index >= mTripods.size())
        return Tripod{ "", 0, "" };
    return mTripods[index];
}

const Rune* Skill::getRune() const
{
    return mRune;
}

void Skill::setName(QString name)
{
    mName = name;
}

void Skill::setIconPath(QString iconPath)
{
    mIconPath = iconPath;
}

void Skill::setLevel(int level)
{
    mLevel = level;
}

void Skill::setRune(Rune* rune)
{
    mRune = rune;
}

void Skill::addTripod(Tripod tripod)
{
    int index = mTripods.size();
    if (index == 0)
        tripod.color = "#31ABD9";
    else if (index == 1)
        tripod.color = "#7EC809";
    else if (index == 2)
        tripod.color = "#FFB000";

    mTripods.append(tripod);
}
