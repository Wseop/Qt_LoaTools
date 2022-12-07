#ifndef ABILITYSTONE_H
#define ABILITYSTONE_H

#include "item.h"

class AbilityStone : public Item
{
public:
    AbilityStone();

    const QList<QPair<QString, int>>& getEngraves() const;
    const QPair<QString, int>& getPenalty() const;

    void addEngrave(QString engrave, int value);
    void setPenalty(QString penalty, int value);

private:
    QList<QPair<QString, int>> m_engraves;
    QPair<QString, int> m_penalty;
};

#endif // ABILITYSTONE_H
