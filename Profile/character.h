#ifndef CHARACTER_H
#define CHARACTER_H

#include "item/equip.h"
#include "item/accessory.h"
#include "item/abilitystone.h"
#include "item/bracelet.h"
#include "item/gem.h"
#include "engrave.h"

class Character
{
public:
    Character();

    QString getName();
    QString getClass();
    QString getLevel();
    QString getServer();
    double getItemLevel();

    Equip& getWeapon();
    Equip& getHead();
    Equip& getTop();
    Equip& getBottom();
    Equip& getHand();
    Equip& getShoulder();
    Accessory& getNeck();
    Accessory& getEar1();
    Accessory& getEar2();
    Accessory& getRing1();
    Accessory& getRing2();
    AbilityStone& getStone();
    Bracelet& getBracelet();
    QList<Gem>& getGems();
    Engrave& getEngrave();

    Item& getItemByPart(Part part);

    void setName(QString name);
    void setClass(QString cls);
    void setLevel(QString level);
    void setServer(QString server);
    void setItemLevel(double itemLevel);

private:
    QString mName;
    QString mClass;
    QString mLevel;
    QString mServer;
    double mItemLevel;

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
};

#endif // CHARACTER_H