#ifndef ABILITYSTONE_H
#define ABILITYSTONE_H

#include "item.h"
#include <QStringList>

class AbilityStone : public Item
{
public:
    AbilityStone();

    const QStringList& getEngraves() const;
    QString getPenalty() const;

    void addEngrave(QString engrave);
    void setPenalty(QString penalty);

private:
    QStringList mEngraves;
    QString mPenalty;
};

#endif // ABILITYSTONE_H
