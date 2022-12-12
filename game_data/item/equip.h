#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"
#include "game_data/item/enum/set_effect.h"

class Equip : public Item
{
public:
    Equip(ItemType type);

    QString getLevelTier() const;
    int getQuality() const;
    SetEffect getSetEffect() const;
    int getSetLevel() const;
    bool isElla() const;

    void setLevelTier(const QString& levelTier);
    void setQuality(int quality);
    void setSetEffect(SetEffect setEffect);
    void setSetLevel(int setLevel);
    void setElla(bool bElla);

private:
    QString m_levelTier;
    int m_quality;
    SetEffect m_setEffect;
    int m_setLevel;
    bool m_bElla;
};

#endif // EQUIP_H
