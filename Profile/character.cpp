#include "character.h"

Character::Character()
    : mItemLevelTotal(0)
{
    mSetEffects.resize(6);
}

Character::~Character()
{
    delete m_pWeapon;
    delete m_pHead;
    delete m_pTop;
    delete m_pBottom;
    delete m_pHand;
    delete m_pShoulder;
    delete m_pNeck;
    delete m_pEar1;
    delete m_pEar2;
    delete m_pRing1;
    delete m_pRing2;
    delete m_pStone;
    delete m_pBracelet;

    for (Skill& skill : mSkills)
    {
        const Rune* rune = skill.getRune();
        if (rune != nullptr)
            delete rune;
    }
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

const Item *Character::getItemByPart(Part part)
{
    if (part == Part::WEAPON)
        return static_cast<Item*>(m_pWeapon);
    else if (part == Part::HEAD)
        return static_cast<Item*>(m_pHead);
    else if (part == Part::TOP)
        return static_cast<Item*>(m_pTop);
    else if (part == Part::BOTTOM)
        return static_cast<Item*>(m_pBottom);
    else if (part == Part::HAND)
        return static_cast<Item*>(m_pHand);
    else if (part == Part::SHOULDER)
        return static_cast<Item*>(m_pShoulder);
    else if (part == Part::NECK)
        return static_cast<Item*>(m_pNeck);
    else if (part == Part::EAR1)
        return static_cast<Item*>(m_pEar1);
    else if (part == Part::EAR2)
        return static_cast<Item*>(m_pEar2);
    else if (part == Part::RING1)
        return static_cast<Item*>(m_pRing1);
    else if (part == Part::RING2)
        return static_cast<Item*>(m_pRing2);
    else if (part == Part::STONE)
        return static_cast<Item*>(m_pStone);
    else if (part == Part::BRACELET)
        return static_cast<Item*>(m_pBracelet);
    else
        return nullptr;
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

void Character::setItem(Item *item)
{
    Part part = item->getPart();

    if (part == Part::WEAPON)
        m_pWeapon = static_cast<Equip*>(item);
    else if (part == Part::HEAD)
        m_pHead = static_cast<Equip*>(item);
    else if (part == Part::TOP)
        m_pTop = static_cast<Equip*>(item);
    else if (part == Part::BOTTOM)
        m_pBottom = static_cast<Equip*>(item);
    else if (part == Part::HAND)
        m_pHand = static_cast<Equip*>(item);
    else if (part == Part::SHOULDER)
        m_pShoulder = static_cast<Equip*>(item);
    else if (part == Part::NECK)
        m_pNeck = static_cast<Accessory*>(item);
    else if (part == Part::EAR1)
        m_pEar1 = static_cast<Accessory*>(item);
    else if (part == Part::EAR2)
        m_pEar2 = static_cast<Accessory*>(item);
    else if (part == Part::RING1)
        m_pRing1 = static_cast<Accessory*>(item);
    else if (part == Part::RING2)
        m_pRing2 = static_cast<Accessory*>(item);
    else if (part == Part::STONE)
        m_pStone = static_cast<AbilityStone*>(item);
    else if (part == Part::BRACELET)
        m_pBracelet = static_cast<Bracelet*>(item);
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
