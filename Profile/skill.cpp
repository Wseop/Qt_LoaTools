#include "skill.h"

Skill::Skill()
{

}

QString Skill::getName()
{
    return mName;
}

QString Skill::getIconPath()
{
    return mIconPath;
}

int Skill::getLevel()
{
    return mLevel;
}

QList<Tripod> Skill::getTripods()
{
    return mTripods;
}

Tripod Skill::getTripod(int index)
{
    return mTripods[index];
}

Rune Skill::getRune()
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

void Skill::setTripod(QList<Tripod> tripods)
{
    mTripods = tripods;
}

void Skill::setRune(Rune rune)
{
    mRune = rune;
}

void Skill::addTripod(Tripod tripod)
{
    mTripods.append(tripod);
}
