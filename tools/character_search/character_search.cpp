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
#include "tools/character_search/ui/others.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <algorithm>

CharacterSearch* CharacterSearch::m_pCharacterSearch = nullptr;
QRegularExpression CharacterSearch::m_regExpHtmlTag("<[^>]*>");

CharacterSearch::CharacterSearch() :
    ui(new Ui::CharacterSearch),
    m_pCharacter(nullptr),
    m_replyHandleStatus(0x00),
    m_pOthers(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();
    ui->groupCharacter->hide();

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
    connect(ui->pbOthers, &QPushButton::pressed, [&](){
        this->setDisabled(true);
        m_pOthers->show();
    });
}

void CharacterSearch::initAlignment()
{
    ui->vLayoutBase->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->vLayoutMain->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->hLayoutGroupSearch->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->hLayoutGroupCharacter->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->vLayoutProfile->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
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

    if (m_pOthers != nullptr)
        delete m_pOthers;

    m_replyHandleStatus = 0x00;

    ui->pbSearch->setDisabled(true);

    QStringList params;
    params << ui->leCharacterName->text();
    ui->leCharacterName->clear();

    for (int i = 0; i < NETWORK_POOL_COUNT; i++)
    {
        QNetworkAccessManager* networkManager = m_networkManagers[i];
        HttpClient::getInstance()->sendGetRequest(networkManager, static_cast<LostarkApi>(i), params);
    }
}

void CharacterSearch::updateStatus(uint8_t statusBit)
{
    m_replyHandleStatus |= statusBit;
    if (m_replyHandleStatus == REPLY_HANDLE_FINISHED)
    {
        ui->pbSearch->setEnabled(true);

        ui->groupCharacter->show();
        m_pOthers = new Others(this, m_pCharacter->getOthers());
    }
}

void CharacterSearch::handleCharacters(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        QMessageBox msgBox;
        msgBox.setText("존재하지 않는 캐릭터입니다.");
        msgBox.exec();
        CharacterSearch::getInstance()->ui->pbSearch->setEnabled(true);
        return;
    }

    const QJsonArray& characters = response.array();
    QList<Other> others;
    for (const QJsonValue& value : characters)
    {
        const QJsonObject& character = value.toObject();
        QString server = character.find("ServerName")->toString();
        int characterLevel = character.find("CharacterLevel")->toInt();
        QString characterName = character.find("CharacterName")->toString();
        Class cls = strToClass(character.find("CharacterClassName")->toString());
        double itemLevel = character.find("ItemMaxLevel")->toString().remove(",").toDouble();
        others.append({server, characterLevel, characterName, cls, itemLevel});
    }

    // sort by level
    std::sort(others.begin(), others.end(), [](Other& a, Other& b) {
        return a.itemLevel > b.itemLevel;
    });

    for (const Other& other : others)
    {
        CharacterSearch::getInstance()->m_pCharacter->addOther(other);
    }

    CharacterSearch::getInstance()->updateStatus(1 << 0);
}

void CharacterSearch::handleProfiles(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
        return;

    const QJsonObject& profileObj = response.object();
    Profile* profile = new Profile();
    profile->setClass(strToClass(profileObj.find("CharacterClassName")->toString()));
    profile->setCharacterLevel(profileObj.find("CharacterLevel")->toInt());
    profile->setCharacterName(profileObj.find("CharacterName")->toString());
    profile->setServer(profileObj.find("ServerName")->toString());
    profile->setTitle(profileObj.find("Title")->toString());
    profile->setGuild(profileObj.find("GuildName")->toString());
    profile->setExpLevel(profileObj.find("ExpeditionLevel")->toInt());
    profile->setItemLevel(profileObj.find("ItemMaxLevel")->toString().remove(",").toDouble());

    const QJsonArray& abilities = profileObj.find("Stats")->toArray();
    for (const QJsonValue& value : abilities)
    {
        const QJsonObject& abilityObj = value.toObject();
        QString abilityStr = abilityObj.find("Type")->toString();
        Ability ability = strToAbility(abilityStr);
        if (ability != Ability::Size)
        {
            int abilityValue = abilityObj.find("Value")->toString().toInt();
            profile->addAbility(ability, abilityValue);
        }
    }

    CharacterSearch::getInstance()->m_pCharacter->setProfile(profile);
    CharacterSearch::getInstance()->updateStatus(1 << 1);
}

void CharacterSearch::handleEquipments(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->updateStatus(1 << 2);
        return;
    }

    const QJsonArray& equipments = response.array();
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

            // remove escape sequences by casting twice (to string -> to object)
            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QJsonObject& itemTitleObj = tooltipObj.find("Element_001")->toObject().find("value")->toObject();
            equip->setLevelTier(itemTitleObj.find("leftStr2")->toString().remove(m_regExpHtmlTag));
            equip->setQuality(itemTitleObj.find("qualityValue")->toInt());

            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();

                if (type == "ItemPartBox")
                {
                    const QJsonObject& valueObj = elementObj.find("value")->toObject();
                    if (valueObj.find("Element_000")->toString().contains("세트 효과 레벨"))
                    {
                        const QString& setEffectLevel = valueObj.find("Element_001")->toString();
                        equip->setSetEffect(strToSetEffect(setEffectLevel.sliced(0, 2)));
                        equip->setSetLevel(setEffectLevel.sliced(setEffectLevel.indexOf("Lv.") + 3, 1).toInt());
                    }
                }
                else if (type == "SingleTextBox")
                {
                    const QString& valueStr = elementObj.find("value")->toString();
                    if (valueStr.contains("엘라 부여 완료"))
                        equip->setElla(true);
                }
            }
            if (equip->getGrade() == ItemGrade::에스더)
                equip->setSetEffect(SetEffect::에스더);

            CharacterSearch::getInstance()->m_pCharacter->setEquip(itemType, equip);
        }
        else if (itemType >= ItemType::투구 && itemType <= ItemType::어깨)
        {
            Equip* equip = new Equip(itemType);
            equip->setName(equipObj.find("Name")->toString());
            equip->setIconPath(equipObj.find("Icon")->toString());
            equip->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QJsonObject& itemTitleObj = tooltipObj.find("Element_001")->toObject().find("value")->toObject();
            equip->setLevelTier(itemTitleObj.find("leftStr2")->toString().remove(m_regExpHtmlTag));
            equip->setQuality(itemTitleObj.find("qualityValue")->toInt());

            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();
                if (type == "ItemPartBox")
                {
                    const QJsonObject& valueObj = elementObj.find("value")->toObject();
                    if (valueObj.find("Element_000")->toString().contains("세트 효과 레벨"))
                    {
                        const QString& setEffectLevel = valueObj.find("Element_001")->toString();
                        equip->setSetEffect(strToSetEffect(setEffectLevel.sliced(0, 2)));
                        equip->setSetLevel(setEffectLevel.sliced(setEffectLevel.indexOf("Lv.") + 3, 1).toInt());
                    }
                }
            }

            CharacterSearch::getInstance()->m_pCharacter->setEquip(itemType, equip);
        }
        else if (itemType == ItemType::목걸이)
        {
            Accessory* accessory = new Accessory(itemType);
            accessory->setName(equipObj.find("Name")->toString());
            accessory->setIconPath(equipObj.find("Icon")->toString());
            accessory->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            accessory->setQuality(tooltipObj.find("Element_001")->toObject().find("value")->toObject().find("qualityValue")->toInt());

            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();
                if (type == "ItemPartBox")
                {
                    // 특성
                    const QJsonObject& valueObj = elementObj.find("value")->toObject();
                    if (valueObj.find("Element_000")->toString().contains("추가 효과"))
                    {
                        const QString& abilitiesStr = valueObj.find("Element_001")->toString();
                        int startIdx, endIdx;
                        QString abilityStr;
                        int abilityValue = 0;

                        // 특성1
                        startIdx = 0;
                        endIdx = abilitiesStr.indexOf("<BR>");
                        abilityStr = abilitiesStr.sliced(startIdx, 2);
                        startIdx = abilitiesStr.indexOf("+") + 1;
                        abilityValue = abilitiesStr.sliced(startIdx, endIdx - startIdx).toInt();
                        accessory->addAbility(strToAbility(abilityStr), abilityValue);
                        // 특성2
                        startIdx = endIdx + 4;
                        endIdx = abilitiesStr.size();
                        abilityStr = abilitiesStr.sliced(startIdx, 2);
                        startIdx = abilitiesStr.indexOf("+", startIdx) + 1;
                        abilityValue = abilitiesStr.sliced(startIdx, endIdx - startIdx).toInt();
                        accessory->addAbility(strToAbility(abilityStr), abilityValue);
                    }
                }
                else if (type == "IndentStringGroup")
                {
                    // 각인
                    const QJsonObject& engravesObj = elementObj.find("value")->toObject()
                                                              .find("Element_000")->toObject()
                                                              .find("contentStr")->toObject();
                    const QStringList& keys = engravesObj.keys();
                    for (const QString& key : keys)
                    {
                        const QString& engraveStr = engravesObj.find(key)->toObject().find("contentStr")->toString();
                        // 각인명
                        int startIdx = engraveStr.indexOf(">") + 1;
                        int endIdx = engraveStr.indexOf("</FONT>");
                        QString engrave = engraveStr.sliced(startIdx, endIdx - startIdx);
                        // 각인값
                        int valueIdx = engraveStr.indexOf("+") + 1;
                        int engraveValue = engraveStr.sliced(valueIdx, 1).toInt();

                        accessory->addEngrave(engrave, engraveValue);
                    }
                }
            }

            CharacterSearch::getInstance()->m_pCharacter->setAccessory(itemType, accessory);
        }
        else if (itemType == ItemType::귀걸이 || itemType == ItemType::반지)
        {
            Accessory* accessory = new Accessory(itemType);
            accessory->setName(equipObj.find("Name")->toString());
            accessory->setIconPath(equipObj.find("Icon")->toString());
            accessory->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            accessory->setQuality(tooltipObj.find("Element_001")->toObject().find("value")->toObject().find("qualityValue")->toInt());

            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();
                if (type == "ItemPartBox")
                {
                    // 특성
                    const QJsonObject& valueObj = elementObj.find("value")->toObject();
                    if (valueObj.find("Element_000")->toString().contains("추가 효과"))
                    {
                        const QString& abilityStr = valueObj.find("Element_001")->toString();
                        QString ability = abilityStr.sliced(0, 2);
                        int index = abilityStr.indexOf("+") + 1;
                        int abilityValue = abilityStr.sliced(index, abilityStr.size() - index).toInt();
                        accessory->addAbility(strToAbility(ability), abilityValue);
                    }
                }
                else if (type == "IndentStringGroup")
                {
                    // 각인
                    const QJsonObject& engravesObj = elementObj.find("value")->toObject()
                                                              .find("Element_000")->toObject()
                                                              .find("contentStr")->toObject();
                    const QStringList& keys = engravesObj.keys();
                    for (const QString& key : keys)
                    {
                        const QString& engraveStr = engravesObj.find(key)->toObject().find("contentStr")->toString();
                        // 각인명
                        int startIdx = engraveStr.indexOf(">") + 1;
                        int endIdx = engraveStr.indexOf("</FONT>");
                        QString engrave = engraveStr.sliced(startIdx, endIdx - startIdx);
                        // 각인값
                        int valueIdx = engraveStr.indexOf("+") + 1;
                        int engraveValue = engraveStr.sliced(valueIdx, 1).toInt();

                        accessory->addEngrave(engrave, engraveValue);
                    }
                }
            }

            CharacterSearch::getInstance()->m_pCharacter->setAccessory(itemType, accessory);
        }
        else if (itemType == ItemType::어빌리티_스톤)
        {
            AbilityStone* stone = new AbilityStone();
            stone->setName(equipObj.find("Name")->toString());
            stone->setIconPath(equipObj.find("Icon")->toString());
            stone->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();
                if (type == "IndentStringGroup")
                {
                    const QJsonObject& engravesObj = elementObj.find("value")->toObject()
                                                               .find("Element_000")->toObject()
                                                               .find("contentStr")->toObject();
                    for (int i = 0; i < 2; i++)
                    {
                        QString key = QString("Element_00%1").arg(i);
                        const QString& engraveStr = engravesObj.find(key)->toObject().find("contentStr")->toString();

                        int startIdx = engraveStr.indexOf(">") + 1;
                        int endIdx = engraveStr.indexOf("</FONT>");
                        QString engrave = engraveStr.sliced(startIdx, endIdx - startIdx);

                        startIdx = engraveStr.indexOf("+") + 1;
                        endIdx = engraveStr.indexOf("<BR>");
                        int engraveValue = engraveStr.sliced(startIdx, endIdx - startIdx).toInt();

                        stone->addEngrave(engrave, engraveValue);
                    }

                    const QString& penaltyStr = engravesObj.find("Element_002")->toObject().find("contentStr")->toString();

                    int startIdx = penaltyStr.indexOf(">") + 1;
                    int endIdx = penaltyStr.indexOf("</FONT>");
                    QString penalty = penaltyStr.sliced(startIdx, endIdx - startIdx);

                    startIdx = penaltyStr.indexOf("+") + 1;
                    endIdx = penaltyStr.indexOf("<BR>");
                    int penaltyValue = penaltyStr.sliced(startIdx, endIdx - startIdx).toInt();

                    stone->setPenalty(penalty, penaltyValue);
                }
            }

            CharacterSearch::getInstance()->m_pCharacter->setAbilityStone(stone);
        }
        else if (itemType == ItemType::팔찌)
        {
            Bracelet* bracelet = new Bracelet();
            bracelet->setName(equipObj.find("Name")->toString());
            bracelet->setIconPath(equipObj.find("Icon")->toString());
            bracelet->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QStringList& elements = tooltipObj.keys();
            for (const QString& element : elements)
            {
                const QJsonObject& elementObj = tooltipObj.find(element)->toObject();
                const QString& type = elementObj.find("type")->toString();
                if (type == "ItemPartBox")
                {
                    const QJsonObject& valueObj = elementObj.find("value")->toObject();
                    if (valueObj.find("Element_000")->toString().contains("팔찌 효과"))
                    {
                        QString effect = valueObj.find("Element_001")->toString();
                        effect.replace("</img>", "@");
                        effect.remove(m_regExpHtmlTag);

                        int startIdx = effect.indexOf("@");
                        int endIdx;
                        while (startIdx != -1)
                        {
                            startIdx++;
                            if (effect[startIdx] == ' ')
                                startIdx++;
                            endIdx = effect.indexOf("@", startIdx);
                            if (endIdx == -1)
                                endIdx = effect.size();

                            bracelet->addEffect(effect.sliced(startIdx, endIdx - startIdx));

                            startIdx = effect.indexOf("@", startIdx);
                        }
                    }
                }
            }

            CharacterSearch::getInstance()->m_pCharacter->setBracelet(bracelet);
        }
    }

    CharacterSearch::getInstance()->updateStatus(1 << 2);
}

void CharacterSearch::handleSkills(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->updateStatus(1 << 3);
        return;
    }

    const QJsonArray& skills = response.array();
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

    CharacterSearch::getInstance()->updateStatus(1 << 3);
}

void CharacterSearch::handleEngraves(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->updateStatus(1 << 4);
        return;
    }

    const QJsonArray& engraves = response.object().find("Effects")->toArray();
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
    CharacterSearch::getInstance()->updateStatus(1 << 4);
}

void CharacterSearch::handleCards(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->updateStatus(1 << 5);
        return;
    }

    const QJsonArray& cards = response.object().find("Effects")->toArray();
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
    CharacterSearch::getInstance()->updateStatus(1 << 5);
}

void CharacterSearch::handleGems(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->updateStatus(1 << 6);
        return;
    }

    QMap<int, Gem*> slotToGem;
    const QJsonObject& gemData = response.object();
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
        slotToGem[slot] = gem;
    }

    for (const QJsonValue& effectValue : gemEffects)
    {
        const QJsonObject& effectObj = effectValue.toObject();
        int slot = effectObj.find("GemSlot")->toInt();
        QString effectStr = QString("%1 %2").arg(effectObj.find("Name")->toString(), effectObj.find("Description")->toString());

        Gem* gem = slotToGem[slot];
        gem->setEffect(effectStr);

        CharacterSearch::getInstance()->m_pCharacter->addGem(gem);
    }

    CharacterSearch::getInstance()->updateStatus(1 << 6);
}

void CharacterSearch::changeCharacter(QString name)
{
    ui->leCharacterName->setText(name);
    sendRequests();
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
