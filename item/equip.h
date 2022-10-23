#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"

class Equip : public Item
{
public:
    Equip(Part part);

    QString getLevelTier();
    int getQuality();
    QString getSetLevel();

    void setLevelTier(QString levelTier);
    void setQuality(int quality);
    void setSetLevel(QString setLevel);

private:
    QString mLevelTier;
    int mQuality;
    QString mSetLevel;
};

#endif // EQUIP_H
