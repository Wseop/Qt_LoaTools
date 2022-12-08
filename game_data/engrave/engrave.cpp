#include "engrave.h"

Engrave::Engrave()
{

}

const QMap<QString, int>& Engrave::getEngraves() const
{
    return m_engraves;
}

const QMap<QString, int>& Engrave::getPenalties() const
{
    return m_penalties;
}

void Engrave::addEngrave(const QString& engrave, int level)
{
    m_engraves[engrave] = level;
}

void Engrave::addPenalty(const QString& penalty, int level)
{
    m_penalties[penalty] = level;
}
