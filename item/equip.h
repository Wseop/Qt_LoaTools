#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"

class Equip : public Item
{
public:
    Equip(Part part);

    QString getLevelTier() const;
    int getQuality() const;
    QString getSetLevel() const;

    void setLevelTier(QString levelTier);
    void setQuality(int quality);
    void setSetLevel(QString setLevel);

private:
    QString mLevelTier;
    int mQuality;
    QString mSetLevel;
};

#endif // EQUIP_H
