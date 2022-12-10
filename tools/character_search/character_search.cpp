#include "character_search.h"
#include "ui_character_search.h"
#include "http_client/http_client.h"
#include "game_data/character/character.h"
#include "game_data/profile/profile.h"
#include "game_data/item/equip.h"
#include "game_data/item/accessory.h"
#include "game_data/item/abilitystone.h"
#include "game_data/item/bracelet.h"
#include "game_data/item/gem.h"
#include "game_data/skill/skill.h"
#include "game_data/engrave/engrave.h"
#include "game_data/engrave/engrave_manager.h"
#include "game_data/card/card.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CharacterSearch* CharacterSearch::m_pCharacterSearch = nullptr;
QRegularExpression CharacterSearch::m_regExpHtmlTag("<[^>]*>");

CharacterSearch::CharacterSearch() :
    ui(new Ui::CharacterSearch),
    m_pCharacter(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();

    initAlignment();
    initNetworkManagerPool();
    initNetworkReplyHandlers();
    initConnect();
}

CharacterSearch::~CharacterSearch()
{
    if (m_pCharacter != nullptr)
    {
        delete m_pCharacter;
        m_pCharacter = nullptr;
    }

    for (int i = 0; i < NETWORK_POOL_COUNT; i++)
        delete m_networkManagers[i];
    m_networkManagers.clear();

    delete ui;
    destroyInstance();
}

void CharacterSearch::initConnect()
{
    connect(ui->pbSearch, &QPushButton::pressed, this, &CharacterSearch::sendRequests);
    connect(ui->leCharacterName, &QLineEdit::returnPressed, this, &CharacterSearch::sendRequests);
    for (int i = 0; i < NETWORK_POOL_COUNT; i++)
        connect(m_networkManagers[i], &QNetworkAccessManager::finished, m_replyHandlers[i]);
}

void CharacterSearch::initAlignment()
{
    ui->vLayoutMain->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->hLayoutSearch->setAlignment(Qt::AlignHCenter);
}

void CharacterSearch::initNetworkManagerPool()
{
    for (int i = 0; i < NETWORK_POOL_COUNT; i++)
    {
        m_networkManagers.append(new QNetworkAccessManager());
    }
}

void CharacterSearch::initNetworkReplyHandlers()
{
    m_replyHandlers.append(handleCharacters);
    m_replyHandlers.append(handleProfiles);
    m_replyHandlers.append(handleEquipments);
    m_replyHandlers.append(handleSkills);
    m_replyHandlers.append(handleEngraves);
    m_replyHandlers.append(handleCards);
    m_replyHandlers.append(handleGems);
}

void CharacterSearch::sendRequests()
{
    if (m_pCharacter != nullptr)
        delete m_pCharacter;
    m_pCharacter = new Character();

    // TODO. enable button when finish handlers
    //ui->pbSearch->setDisabled(true);

    QStringList params;
    params << ui->leCharacterName->text();

    for (int i = 0; i < NETWORK_POOL_COUNT; i++)
    {
        QNetworkAccessManager* networkManager = m_networkManagers[i];
        HttpClient::getInstance()->sendGetRequest(networkManager, static_cast<LostarkApi>(i), params);
    }
}

void CharacterSearch::handleCharacters(QNetworkReply* reply)
{
    const QJsonArray& characters = QJsonDocument::fromJson(reply->readAll()).array();

    for (const QJsonValue& value : characters)
    {
        const QJsonObject& character = value.toObject();
        QString server = character.find("ServerName")->toString();
        int characterLevel = character.find("CharacterLevel")->toInt();
        QString characterName = character.find("CharacterName")->toString();
        Class cls = strToClass(character.find("CharacterClassName")->toString());
        double itemLevel = character.find("ItemMaxLevel")->toDouble();

        CharacterSearch::getInstance()->m_pCharacter->addOther({server, characterLevel, characterName, cls, itemLevel});
    }
}

void CharacterSearch::handleProfiles(QNetworkReply* reply)
{
    const QJsonObject& profileObj = QJsonDocument::fromJson(reply->readAll()).object();

    Profile* profile = new Profile();
    profile->setClass(strToClass(profileObj.find("CharacterClassName")->toString()));
    profile->setCharacterLevel(profileObj.find("CharacterLevel")->toInt());
    profile->setCharacterName(profileObj.find("CharacterName")->toString());
    profile->setServer(profileObj.find("ServerName")->toString());
    profile->setTitle(profileObj.find("Title")->toString());
    profile->setGuild(profileObj.find("GuildName")->toString());
    profile->setExpLevel(profileObj.find("ExpeditionLevel")->toInt());
    profile->setItemLevel(profileObj.find("ItemMaxLevel")->toDouble());

    const QJsonArray& abilities = profileObj.find("Stats")->toArray();
    for (const QJsonValue& value : abilities)
    {
        const QJsonObject& abilityObj = value.toObject();
        QString abilityStr = abilityObj.find("Type")->toString();
        Ability ability = strToAbility(abilityStr);
        if (ability != Ability::Size)
        {
            int abilityValue = abilityObj.find("Value")->toInt();
            profile->addAbility(ability, abilityValue);
        }
    }

    CharacterSearch::getInstance()->m_pCharacter->setProfile(profile);
}

void CharacterSearch::handleEquipments(QNetworkReply* reply)
{
    const QJsonArray& equipments = QJsonDocument::fromJson(reply->readAll()).array();

    for (const QJsonValue& value : equipments)
    {
        const QJsonObject& equipObj = value.toObject();
        ItemType itemType = strToItemType(equipObj.find("Type")->toString());
        if (itemType == ItemType::무기)
        {
            Equip* equip = new Equip(itemType);
            equip->setName(equipObj.find("Name")->toString());
            equip->setIconPath(equipObj.find("Icon")->toString());
            equip->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setEquip(itemType, equip);
        }
        else if (itemType >= ItemType::투구 && itemType <= ItemType::어깨)
        {
            Equip* equip = new Equip(itemType);
            equip->setName(equipObj.find("Name")->toString());
            equip->setIconPath(equipObj.find("Icon")->toString());
            equip->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setEquip(itemType, equip);
        }
        else if (itemType == ItemType::목걸이)
        {
            Accessory* accessory = new Accessory(itemType);
            accessory->setName(equipObj.find("Name")->toString());
            accessory->setIconPath(equipObj.find("Icon")->toString());
            accessory->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setAccessory(itemType, accessory);
        }
        else if (itemType == ItemType::귀걸이 || itemType == ItemType::반지)
        {
            Accessory* accessory = new Accessory(itemType);
            accessory->setName(equipObj.find("Name")->toString());
            accessory->setIconPath(equipObj.find("Icon")->toString());
            accessory->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setAccessory(itemType, accessory);
        }
        else if (itemType == ItemType::어빌리티_스톤)
        {
            AbilityStone* stone = new AbilityStone();
            stone->setName(equipObj.find("Name")->toString());
            stone->setIconPath(equipObj.find("Icon")->toString());
            stone->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setAbilityStone(stone);
        }
        else if (itemType == ItemType::팔찌)
        {
            Bracelet* bracelet = new Bracelet();
            bracelet->setName(equipObj.find("Name")->toString());
            bracelet->setIconPath(equipObj.find("Icon")->toString());
            bracelet->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));
            // TODO. parse tooltip
            CharacterSearch::getInstance()->m_pCharacter->setBracelet(bracelet);
        }
    }
}

void CharacterSearch::handleSkills(QNetworkReply* reply)
{
    const QJsonArray& skills = QJsonDocument::fromJson(reply->readAll()).array();

    for (const QJsonValue& value : skills)
    {
        const QJsonObject& skillObj = value.toObject();
        int level = skillObj.find("Level")->toInt();
        bool bRune = !skillObj.find("Rune")->isNull();
        if (level > 1 || bRune)
        {
            Skill* skill = new Skill();
            skill->setName(skillObj.find("Name")->toString());
            skill->setIconPath(skillObj.find("Icon")->toString());
            skill->setLevel(level);

            const QJsonArray& tripods = skillObj.find("Tripods")->toArray();
            for (const QJsonValue& tripodValue : tripods)
            {
                const QJsonObject& tripodObj = tripodValue.toObject();
                if (tripodObj.find("IsSelected")->toBool())
                {
                    Tripod tripod;
                    tripod.name = tripodObj.find("Name")->toString();
                    tripod.tier = tripodObj.find("Tier")->toInt();
                    tripod.level = tripodObj.find("Level")->toInt();
                    skill->addTripod(tripod);
                }
            }

            if (bRune)
            {
                const QJsonObject& runeObj = skillObj.find("Rune")->toObject();
                QString name = runeObj.find("Name")->toString();
                ItemGrade grade = strToItemGrade(runeObj.find("Grade")->toString());
                QString iconPath = runeObj.find("Icon")->toString();

                SkillRune* rune = new SkillRune(name, grade, iconPath);
                skill->setRune(rune);
            }

            CharacterSearch::getInstance()->m_pCharacter->addSkill(skill);
        }
    }
}

void CharacterSearch::handleEngraves(QNetworkReply* reply)
{
    const QJsonArray& engraves = QJsonDocument::fromJson(reply->readAll()).object().find("Effects")->toArray();
    Engrave* engrave = new Engrave();

    for (const QJsonValue& value : engraves)
    {
        const QJsonObject& engraveObj = value.toObject();
        QString nameLevel = engraveObj.find("Name")->toString();
        int endIndex = nameLevel.indexOf("Lv.") - 1;

        QString name = nameLevel.sliced(0, endIndex);
        int level = nameLevel.last(1).toInt();
        if (EngraveManager::getInstance()->isValidEngrave(name))
            engrave->addEngrave(name, level);
        else if (EngraveManager::getInstance()->isValidPenalty(name))
            engrave->addPenalty(name, level);
    }

    CharacterSearch::getInstance()->m_pCharacter->setEngrave(engrave);
}

void CharacterSearch::handleCards(QNetworkReply* reply)
{
    const QJsonArray& cards = QJsonDocument::fromJson(reply->readAll()).object().find("Effects")->toArray();
    Card* card = new Card();

    for (const QJsonValue& value : cards)
    {
        const QJsonArray& cardEffects = value.toObject().find("Items")->toArray();
        for (const QJsonValue& effectValue : cardEffects)
        {
            const QJsonObject& effectObj = effectValue.toObject();
            QString name = effectObj.find("Name")->toString();
            QString desc = effectObj.find("Description")->toString();
            card->addCardEffect(name, desc);
        }
    }

    CharacterSearch::getInstance()->m_pCharacter->setCard(card);
}

void CharacterSearch::handleGems(QNetworkReply* reply)
{
    QMap<int, Gem*> slotGems;
    const QJsonObject& gemData = QJsonDocument::fromJson(reply->readAll()).object();
    const QJsonArray& gems = gemData.find("Gems")->toArray();
    const QJsonArray& gemEffects = gemData.find("Effects")->toArray();

    for (const QJsonValue& gemValue : gems)
    {
        const QJsonObject& gemObj = gemValue.toObject();
        int slot = gemObj.find("Slot")->toInt();

        Gem* gem = new Gem();
        gem->setName(gemObj.find("Name")->toString().remove(m_regExpHtmlTag));
        gem->setIconPath(gemObj.find("Icon")->toString());
        gem->setLevel(gemObj.find("Level")->toInt());
        gem->setGrade(strToItemGrade(gemObj.find("Grade")->toString()));
        slotGems[slot] = gem;
    }

    for (const QJsonValue& effectValue : gemEffects)
    {
        const QJsonObject& effectObj = effectValue.toObject();
        int slot = effectObj.find("GemSlot")->toInt();
        QString effectStr = QString("%1 %2").arg(effectObj.find("Name")->toString(), effectObj.find("Description")->toString());

        Gem* gem = slotGems[slot];
        gem->setEffect(effectStr);

        CharacterSearch::getInstance()->m_pCharacter->addGem(gem);
    }
}

CharacterSearch* CharacterSearch::getInstance()
{
    if (m_pCharacterSearch == nullptr)
        m_pCharacterSearch = new CharacterSearch();

    return m_pCharacterSearch;
}

void CharacterSearch::destroyInstance()
{
    if (m_pCharacterSearch == nullptr)
        return;

    delete m_pCharacterSearch;
    m_pCharacterSearch = nullptr;
}
