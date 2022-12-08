#ifndef ABILITYSTONE_H
#define ABILITYSTONE_H

#include "item.h"

class AbilityStone : public Item
{
public:
    AbilityStone();

    const QList<QPair<QString, int>>& getEngraves() const;
    const QPair<QString, int>& getPenalty() const;

    void addEngrave(const QString& engrave, int value);
    void setPenalty(const QString& penalty, int value);

private:
    QList<QPair<QString, int>> m_engraves;
    QPair<QString, int> m_penalty;
};

#endif // ABILITYSTONE_H
