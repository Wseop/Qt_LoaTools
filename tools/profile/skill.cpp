#include "skill.h"

Skill::Skill()
{

}

QString Skill::getName() const
{
    return m_name;
}

QString Skill::getIconPath() const
{
    return m_iconPath;
}

int Skill::getLevel() const
{
    return m_level;
}

QList<Tripod> Skill::getTripods() const
{
    return m_tripods;
}

Tripod Skill::getTripod(int index) const
{
    if (index >= m_tripods.size())
        return Tripod{ "", 0, "" };
    return m_tripods[index];
}

const Rune* Skill::getRune() const
{
    return m_pRune;
}

void Skill::setName(QString name)
{
    m_name = name;
}

void Skill::setIconPath(QString iconPath)
{
    m_iconPath = iconPath;
}

void Skill::setLevel(int level)
{
    m_level = level;
}

void Skill::setRune(Rune* rune)
{
    m_pRune = rune;
}

void Skill::addTripod(Tripod tripod)
{
    int index = m_tripods.size();
    if (index == 0)
        tripod.color = "#31ABD9";
    else if (index == 1)
        tripod.color = "#7EC809";
    else if (index == 2)
        tripod.color = "#FFB000";

    m_tripods.append(tripod);
}
