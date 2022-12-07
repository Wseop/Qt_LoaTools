#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"

class Equip : public Item
{
public:
    Equip(ItemType type);

    int getReforge();
    QString getLevelTier() const;
    int getQuality() const;
    QString getSetEffect() const;
    QString getSetLevel() const;

    void setReforge(int reforge);
    void setLevelTier(QString levelTier);
    void setQuality(int quality);
    void setSetEffect(QString setEffect);
    void setSetLevel(QString setLevel);

private:
    int m_reforge;
    QString m_levelTier;
    int m_quality;
    QString m_setEffect;
    QString m_setLevel;
};

#endif // EQUIP_H
