#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "item.h"
#include "game_data/profile/enum/ability.h"

class Accessory : public Item
{
public:
    Accessory(ItemType type);

    int getQuality() const;
    const QList<QPair<Ability, int>>& getAbilities() const;
    const QList<QPair<QString, int>>& getEngraves() const;
    const QPair<QString, int>& getPenalty() const;

    void setQuality(int quality);
    void addAbility(Ability ability, int value);
    void addEngrave(QString engrave, int value);
    void setPenalty(QString penalty, int value);

private:
    int m_quality;
    QList<QPair<Ability, int>> m_abilities;
    QList<QPair<QString, int>> m_engraves;
    QPair<QString, int> m_penalty;
};

#endif // ACCESSORY_H
