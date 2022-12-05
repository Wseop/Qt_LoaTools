#ifndef CHARACTER_H
#define CHARACTER_H

#include "item/equip.h"
#include "item/accessory.h"
#include "item/abilitystone.h"
#include "item/bracelet.h"
#include "item/gem.h"
#include "card_set.h"
#include "skill.h"

class Character
{
public:
    Character();
    ~Character();

    QString getName();
    QString getClass();
    QString getLevel();
    QString getServer();
    QString getGuild();
    double getItemLevel();
    const CardSet& getCardSet();
    const QStringList& getSetEffects();

    const Item* getItemByPart(Part part);
    const QList<Gem>& getGems();

    const QMap<QString, int>& getEngraveValues();
    const QMap<QString, int>& getPenaltyValues();
    const QList<Skill>& getSkills();

    void setName(QString name);
    void setClass(QString cls);
    void setLevel(QString level);
    void setServer(QString server);
    void setGuild(QString guild);
    void addItemLevel(int itemLevel);
    void setCardSet(CardSet cardSet);
    void addSetEffect(QString setEffect, int index);

    void setItem(Item* item);
    void addGem(const Gem& gem);

    void addEngrave(QString name, int value);
    void addPenalty(QString name, int value);

    void addSkill(const Skill& skill);

private:
    QString m_name;
    QString m_class;
    QString m_level;
    QString m_server;
    QString m_guild;
    int m_itemLevelTotal;
    CardSet m_cardSet;
    QStringList m_setEffects;

    Equip* m_pWeapon = nullptr;
    Equip* m_pHead = nullptr;
    Equip* m_pTop = nullptr;
    Equip* m_pBottom = nullptr;
    Equip* m_pHand = nullptr;
    Equip* m_pShoulder = nullptr;
    Accessory* m_pNeck = nullptr;
    Accessory* m_pEar1 = nullptr;
    Accessory* m_pEar2 = nullptr;
    Accessory* m_pRing1 = nullptr;
    Accessory* m_pRing2 = nullptr;
    AbilityStone* m_pStone = nullptr;
    Bracelet* m_pBracelet = nullptr;
    QList<Gem> m_gems;

    QMap<QString, int> m_engraveValues;
    QMap<QString, int> m_penaltyValues;
    QList<Skill> mSkills;
};

#endif // CHARACTER_H
