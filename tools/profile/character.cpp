#include "character.h"

Character::Character()
    : m_itemLevelTotal(0)
{
    m_setEffects.resize(6);
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
    return m_name;
}

QString Character::getClass()
{
    return m_class;
}

QString Character::getLevel()
{
    return m_level;
}

QString Character::getServer()
{
    return m_server;
}

QString Character::getGuild()
{
    return m_guild;
}

int Character::getExpLevel()
{
    return m_expLevel;
}

double Character::getItemLevel()
{
    return (double)m_itemLevelTotal / 6;
}

const CardSet& Character::getCardSet()
{
    return m_cardSet;
}

const QStringList &Character::getSetEffects()
{
    return m_setEffects;
}

const QList<Gem> &Character::getGems()
{
    return m_gems;
}

const QMap<QString, int> &Character::getEngraveValues()
{
    return m_engraveValues;
}

const QMap<QString, int> &Character::getPenaltyValues()
{
    return m_penaltyValues;
}

const QList<Skill> &Character::getSkills()
{
    return mSkills;
}

const Item *Character::getItemByPart(ItemPart part)
{
    if (part == ItemPart::WEAPON)
        return static_cast<Item*>(m_pWeapon);
    else if (part == ItemPart::HEAD)
        return static_cast<Item*>(m_pHead);
    else if (part == ItemPart::TOP)
        return static_cast<Item*>(m_pTop);
    else if (part == ItemPart::BOTTOM)
        return static_cast<Item*>(m_pBottom);
    else if (part == ItemPart::HAND)
        return static_cast<Item*>(m_pHand);
    else if (part == ItemPart::SHOULDER)
        return static_cast<Item*>(m_pShoulder);
    else if (part == ItemPart::NECK)
        return static_cast<Item*>(m_pNeck);
    else if (part == ItemPart::EAR1)
        return static_cast<Item*>(m_pEar1);
    else if (part == ItemPart::EAR2)
        return static_cast<Item*>(m_pEar2);
    else if (part == ItemPart::RING1)
        return static_cast<Item*>(m_pRing1);
    else if (part == ItemPart::RING2)
        return static_cast<Item*>(m_pRing2);
    else if (part == ItemPart::STONE)
        return static_cast<Item*>(m_pStone);
    else if (part == ItemPart::BRACELET)
        return static_cast<Item*>(m_pBracelet);
    else
        return nullptr;
}

void Character::setName(QString name)
{
    m_name = name;
}

void Character::setClass(QString cls)
{
    m_class = cls;
}

void Character::setLevel(QString level)
{
    m_level = level;
}

void Character::setServer(QString server)
{
    m_server = server;
}

void Character::setGuild(QString guild)
{
    m_guild = guild;
}

void Character::setExpLevel(int level)
{
    m_expLevel = level;
}

void Character::addItemLevel(int itemLevel)
{
    m_itemLevelTotal += itemLevel;
}

void Character::setCardSet(CardSet cardSet)
{
    m_cardSet = cardSet;
}

void Character::addSetEffect(QString setEffect, int index)
{
    m_setEffects[index] = setEffect;
}

void Character::setItem(Item *item)
{
    ItemPart part = item->getPart();

    if (part == ItemPart::WEAPON)
        m_pWeapon = static_cast<Equip*>(item);
    else if (part == ItemPart::HEAD)
        m_pHead = static_cast<Equip*>(item);
    else if (part == ItemPart::TOP)
        m_pTop = static_cast<Equip*>(item);
    else if (part == ItemPart::BOTTOM)
        m_pBottom = static_cast<Equip*>(item);
    else if (part == ItemPart::HAND)
        m_pHand = static_cast<Equip*>(item);
    else if (part == ItemPart::SHOULDER)
        m_pShoulder = static_cast<Equip*>(item);
    else if (part == ItemPart::NECK)
        m_pNeck = static_cast<Accessory*>(item);
    else if (part == ItemPart::EAR1)
        m_pEar1 = static_cast<Accessory*>(item);
    else if (part == ItemPart::EAR2)
        m_pEar2 = static_cast<Accessory*>(item);
    else if (part == ItemPart::RING1)
        m_pRing1 = static_cast<Accessory*>(item);
    else if (part == ItemPart::RING2)
        m_pRing2 = static_cast<Accessory*>(item);
    else if (part == ItemPart::STONE)
        m_pStone = static_cast<AbilityStone*>(item);
    else if (part == ItemPart::BRACELET)
        m_pBracelet = static_cast<Bracelet*>(item);
}

void Character::addGem(const Gem &gem)
{
    m_gems.append(gem);
}

void Character::addEngrave(QString name, int value)
{
    m_engraveValues[name] += value;
}

void Character::addPenalty(QString name, int value)
{
    m_penaltyValues[name] += value;
}

void Character::addSkill(const Skill &skill)
{
    mSkills.append(skill);
}