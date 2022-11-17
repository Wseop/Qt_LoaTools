#include "character.h"

Character::Character()
    : mItemLevelTotal(0),
      mWeapon(Equip(Part::WEAPON)),
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
    mSetEffects.resize(6);
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

QString Character::getGuild()
{
    return mGuild;
}

double Character::getItemLevel()
{
    return (double)mItemLevelTotal / 6;
}

const CardSet& Character::getCardSet()
{
    return mCardSet;
}

const QStringList &Character::getSetEffects()
{
    return mSetEffects;
}

const QList<Gem> &Character::getGems()
{
    return mGems;
}

const Engrave &Character::getEngrave()
{
    return mEngrave;
}

const QList<Skill> &Character::getSkills()
{
    return mSkills;
}

const Item& Character::getItemByPart(Part part)
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

void Character::setGuild(QString guild)
{
    mGuild = guild;
}

void Character::addItemLevel(int itemLevel)
{
    mItemLevelTotal += itemLevel;
}

void Character::setCardSet(CardSet cardSet)
{
    mCardSet = cardSet;
}

void Character::addSetEffect(QString setEffect, int index)
{
    mSetEffects[index] = setEffect;
}

void Character::setItem(const Item& item)
{
    Part part = item.getPart();

    if (part == Part::WEAPON)
        mWeapon = static_cast<const Equip&>(item);
    else if (part == Part::HEAD)
        mHead = static_cast<const Equip&>(item);
    else if (part == Part::TOP)
        mTop = static_cast<const Equip&>(item);
    else if (part == Part::BOTTOM)
        mBottom = static_cast<const Equip&>(item);
    else if (part == Part::HAND)
        mHand = static_cast<const Equip&>(item);
    else if (part == Part::SHOULDER)
        mShoulder = static_cast<const Equip&>(item);
    else if (part == Part::NECK)
        mNeck = static_cast<const Accessory&>(item);
    else if (part == Part::EAR1)
        mEar1 = static_cast<const Accessory&>(item);
    else if (part == Part::EAR2)
        mEar2 = static_cast<const Accessory&>(item);
    else if (part == Part::RING1)
        mRing1 = static_cast<const Accessory&>(item);
    else if (part == Part::RING2)
        mRing2 = static_cast<const Accessory&>(item);
    else if (part == Part::STONE)
        mStone = static_cast<const AbilityStone&>(item);
    else if (part == Part::BRACELET)
        mBracelet = static_cast<const Bracelet&>(item);
}

void Character::addGem(const Gem &gem)
{
    mGems.append(gem);
}

void Character::addEngrave(QString name, int value)
{
    mEngrave.addEngrave(name, value);
}

void Character::addPenalty(QString name, int value)
{
    mEngrave.addPenalty(name, value);
}

void Character::addSkill(const Skill &skill)
{
    mSkills.append(skill);
}
