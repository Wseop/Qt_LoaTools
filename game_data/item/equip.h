#ifndef EQUIP_H
#define EQUIP_H

#include "item.h"
#include "game_data/item/enum/set_effect.h"

class Equip : public Item
{
public:
    Equip(ItemType type);

    int getReforge();
    QString getLevelTier() const;
    int getQuality() const;
    SetEffect getSetEffect() const;
    QString getSetLevel() const;

    void setReforge(int reforge);
    void setLevelTier(QString levelTier);
    void setQuality(int quality);
    void setSetEffect(SetEffect setEffect);
    void setSetLevel(QString setLevel);

private:
    int m_reforge;
    QString m_levelTier;
    int m_quality;
    SetEffect m_setEffect;
    QString m_setLevel;
};

#endif // EQUIP_H
