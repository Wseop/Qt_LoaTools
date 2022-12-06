#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"

class Equip : public Item
{
public:
    Equip(ItemPart part);

    QString getLevelTier() const;
    int getQuality() const;
    QString getSetLevel() const;

    void setLevelTier(QString levelTier);
    void setQuality(int quality);
    void setSetLevel(QString setLevel);

private:
    QString m_levelTier;
    int m_quality;
    QString m_setLevel;
};

#endif // EQUIP_H
