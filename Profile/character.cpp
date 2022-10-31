#include "character.h"

Character::Character()
    : mWeapon(Equip(Part::WEAPON)),
      mHead(Equip(Part::HEAD)),
      mTop(Equip(Part::TOP)),
      mBottom(Equip(Part::BOTTOM)),
      mHand(Equip(Part::HAND)),
      mShoulder(Equip(Part::SHOULDER)),
      mNeck(Accessory(Part::NECK)),
      mEar1(Accessory(Part::EAR1)),
      mEar2(Accessory(Part::EAR2)),
      mRing1(Accessory(Part::RING1)),
      mRing2(Accessory(Part::RING2)),
      mStone(AbilityStone()),
      mBracelet(Bracelet())
{

}

QString Character::getName()
{
    return mName;
}

QString Character::getClass()
{
    return mClass;
}

QString Character::getLevel()
{
    return mLevel;
}

QString Character::getServer()
{
    return mServer;
}

double Character::getItemLevel()
{
    return mItemLevel;
}

CardSet& Character::getCardSet()
{
    return mCardSet;
}

Equip &Character::getWeapon()
{
    return mWeapon;
}

Equip &Character::getHead()
{
    return mHead;
}

Equip &Character::getTop()
{
    return mTop;
}

Equip &Character::getBottom()
{
    return mBottom;
}

Equip &Character::getHand()
{
    return mHand;
}

Equip &Character::getShoulder()
{
    return mShoulder;
}

Accessory &Character::getNeck()
{
    return mNeck;
}

Accessory &Character::getEar1()
{
    return mEar1;
}

Accessory &Character::getEar2()
{
    return mEar2;
}

Accessory &Character::getRing1()
{
    return mRing1;
}

Accessory &Character::getRing2()
{
    return mRing2;
}

AbilityStone &Character::getStone()
{
    return mStone;
}

Bracelet &Character::getBracelet()
{
    return mBracelet;
}

QList<Gem> &Character::getGems()
{
    return mGems;
}

Engrave &Character::getEngrave()
{
    return mEngrave;
}

Item& Character::getItemByPart(Part part)
{
    if (part == Part::WEAPON)
        return static_cast<Item&>(mWeapon);
    else if (part == Part::HEAD)
        return static_cast<Item&>(mHead);
    else if (part == Part::TOP)
        return static_cast<Item&>(mTop);
    else if (part == Part::BOTTOM)
        return static_cast<Item&>(mBottom);
    else if (part == Part::HAND)
        return static_cast<Item&>(mHand);
    else if (part == Part::SHOULDER)
        return static_cast<Item&>(mShoulder);
    else if (part == Part::NECK)
        return static_cast<Item&>(mNeck);
    else if (part == Part::EAR1)
        return static_cast<Item&>(mEar1);
    else if (part == Part::EAR2)
        return static_cast<Item&>(mEar2);
    else if (part == Part::RING1)
        return static_cast<Item&>(mRing1);
    else if (part == Part::RING2)
        return static_cast<Item&>(mRing2);
    else if (part == Part::STONE)
        return static_cast<Item&>(mStone);
    else if (part == Part::BRACELET)
        return static_cast<Item&>(mBracelet);
}

void Character::setName(QString name)
{
    mName = name;
}

void Character::setClass(QString cls)
{
    mClass = cls;
}

void Character::setLevel(QString level)
{
    mLevel = level;
}

void Character::setServer(QString server)
{
    mServer = server;
}

void Character::setItemLevel(double itemLevel)
{
    mItemLevel = itemLevel;
}

void Character::setCardSet(CardSet cardSet)
{
    mCardSet = cardSet;
}

