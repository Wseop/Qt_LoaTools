#ifndef CHARACTER_H
#define CHARACTER_H

#include "item/equip.h"
#include "item/accessory.h"
#include "item/abilitystone.h"
#include "item/bracelet.h"
#include "item/gem.h"
#include "engrave.h"
#include "card_set.h"
#include "skill.h"

class Character
{
public:
    Character();

    QString getName();
    QString getClass();
    QString getLevel();
    QString getServer();
    QString getGuild();
    double getItemLevel();
    const CardSet& getCardSet();
    const QStringList& getSetEffects();

    const Item& getItemByPart(Part part);
    const QList<Gem>& getGems();

    const Engrave& getEngrave();
    const QList<Skill>& getSkills();

    void setName(QString name);
    void setClass(QString cls);
    void setLevel(QString level);
    void setServer(QString server);
    void setGuild(QString guild);
    void addItemLevel(int itemLevel);
    void setCardSet(CardSet cardSet);
    void addSetEffect(QString setEffect, int index);

    void setItem(const Item& item);
    void addGem(const Gem& gem);

    void addEngrave(QString name, int value);
    void addPenalty(QString name, int value);

    void addSkill(const Skill& skill);

private:
    QString mName;
    QString mClass;
    QString mLevel;
    QString mServer;
    QString mGuild;
    int mItemLevelTotal;
    CardSet mCardSet;
    QStringList mSetEffects;

    Equip mWeapon;
    Equip mHead;
    Equip mTop;
    Equip mBottom;
    Equip mHand;
    Equip mShoulder;
    Accessory mNeck;
    Accessory mEar1;
    Accessory mEar2;
    Accessory mRing1;
    Accessory mRing2;
    AbilityStone mStone;
    Bracelet mBracelet;
    QList<Gem> mGems;

    Engrave mEngrave;
    QList<Skill> mSkills;
};

#endif // CHARACTER_H
