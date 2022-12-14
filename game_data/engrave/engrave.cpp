#include "engrave.h"

Engrave::Engrave()
{

}

const QList<QPair<QString, int>>& Engrave::getEngraves() const
{
    return m_engraves;
}

const QList<QPair<QString, int>>& Engrave::getPenalties() const
{
    return m_penalties;
}

void Engrave::addEngrave(const QString& engrave, int level)
{
    m_engraves.append({engrave, level});
}

void Engrave::addPenalty(const QString& penalty, int level)
{
    m_penalties.append({penalty, level});
}
