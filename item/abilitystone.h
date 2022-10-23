#ifndef ABILITYSTONE_H
#define ABILITYSTONE_H

#include "item.h"

class AbilityStone : public Item
{
public:
    AbilityStone();

    QString getEngrave();

    void setEngrave(QString engrave);

private:
    QString mEngrave;
};

#endif // ABILITYSTONE_H
