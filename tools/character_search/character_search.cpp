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
#include "font/font_manager.h"
#include "tools/character_search/ui/others.h"
#include "tools/character_search/ui/profile_widget.h"
#include "tools/character_search/ui/equip_widget.h"
#include "tools/character_search/ui/accessory_widget.h"
#include "tools/character_search/ui/abilitystone_widget.h"
#include "tools/character_search/ui/bracelet_widget.h"
#include "tools/character_search/ui/gem_widget.h"
#include "tools/character_search/ui/engrave_widget.h"
#include "tools/character_search/ui/card_widget.h"
#include "tools/character_search/ui/skill_widget.h"
#include "db/db_request.h"
#include "db/document/document_manager.h"
#include "db/document/settingcode_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QThread>
#include <algorithm>

CharacterSearch* CharacterSearch::m_pCharacterSearch = nullptr;
QRegularExpression CharacterSearch::m_regExpHtmlTag("<[^>]*>");

CharacterSearch::CharacterSearch() :
    ui(new Ui::CharacterSearch),
    m_pCharacter(nullptr),
    m_replyHandleStatus(0x00),
    m_pOthers(nullptr),
    m_pProfileWidget(nullptr),
    m_pStoneWidget(nullptr),
    m_pBraceletWidget(nullptr),
    m_pEngraveWidget(nullptr),
    m_pCardWidget(nullptr),
    m_pDbRequestThread(new QThread()),
    m_pDbRequest(new DBRequest())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();
    ui->groupCharacter->hide();

    initAlignment();
    initStyleSheet();
    initFont();
    initNetworkManagerPool();
    initNetworkReplyHandlers();
    initConnect();

    m_pDbRequest->moveToThread(m_pDbRequestThread);
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
    connect(ui->pbOthers, &QPushButton::pressed, this, [&](){
        this->setDisabled(true);
        m_pOthers->show();
    });
}

void CharacterSearch::initAlignment()
{
    ui->vLayoutBase->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->vLayoutMain->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->hLayoutGroupSearch->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->hLayoutGroupCharacter->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->hLayoutCharacterScroll->setAlignment(Qt::AlignTop);
    ui->vLayoutProfile->setAlignment(Qt::AlignTop);
    ui->vLayoutEquip->setAlignment(Qt::AlignTop);
    ui->vLayoutAccessory->setAlignment(Qt::AlignTop);
    ui->vLayoutEtc->setAlignment(Qt::AlignTop);
    ui->vLayoutGem1->setAlignment(Qt::AlignTop);
    ui->vLayoutGem2->setAlignment(Qt::AlignTop);
    ui->vLayoutSkill->setAlignment(Qt::AlignTop);
}

void CharacterSearch::initStyleSheet()
{
    ui->tab->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tab_2->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
}

void CharacterSearch::initFont()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont nanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->leCharacterName->setFont(nanumBold10);
    ui->pbSearch->setFont(nanumBold10);
    ui->pbOthers->setFont(nanumBold10);
    ui->tabCharacter->setFont(nanumRegular10);
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
    reset();
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

        const Profile* pProfile = m_pCharacter->getProfile();
        if (pProfile != nullptr)
        {
            m_pProfileWidget = new ProfileWidget(this, pProfile);
            ui->vLayoutProfile->addWidget(m_pProfileWidget);
        }

        for (int i = static_cast<int>(ItemType::무기); i <= static_cast<int>(ItemType::어깨); i++)
        {
            ItemType type = static_cast<ItemType>(i);
            const Equip* pEquip = m_pCharacter->getEquip(type);
            if (pEquip == nullptr)
                continue;

            EquipWidget* pEquipWidget = new EquipWidget(this, pEquip);
            m_equipWidgets.append(pEquipWidget);
            ui->vLayoutEquip->addWidget(pEquipWidget);
        }

        const Accessory* pAcc = m_pCharacter->getAccessory(ItemType::목걸이);
        if (pAcc != nullptr)
        {
            AccessoryWidget* pNeckWidget = new AccessoryWidget(this, pAcc);
            m_accessoryWidgets.append(pNeckWidget);
            ui->vLayoutAccessory->addWidget(pNeckWidget);
        }
        for (int i = static_cast<int>(ItemType::귀걸이); i <= static_cast<int>(ItemType::반지); i++)
        {
            for (int j = 0; j < 2; j++)
            {
                ItemType type = static_cast<ItemType>(i);
                pAcc = m_pCharacter->getAccessory(type, j);
                if (pAcc == nullptr)
                    continue;

                AccessoryWidget* pAccWidget = new AccessoryWidget(this, pAcc);
                m_accessoryWidgets.append(pAccWidget);
                ui->vLayoutAccessory->addWidget(pAccWidget);
            }
        }

        const AbilityStone* pStone = m_pCharacter->getAbilityStone();
        if (pStone != nullptr)
        {
            m_pStoneWidget = new AbilityStoneWidget(this, pStone);
            ui->vLayoutEtc->addWidget(m_pStoneWidget);
        }

        const Bracelet* pBracelet = m_pCharacter->getBracelet();
        if (pBracelet != nullptr)
        {
            m_pBraceletWidget = new BraceletWidget(this, pBracelet);
            ui->vLayoutEtc->addWidget(m_pBraceletWidget);
        }

        const QList<Gem*>& gems = m_pCharacter->getGems();
        for (const Gem* gem : gems)
        {
            GemWidget* gemWidget = new GemWidget(this, gem);
            m_gemWidgets.append(gemWidget);
            if (gem->getGemType() == GemType::멸화)
                ui->vLayoutGem1->addWidget(gemWidget);
            else
                ui->vLayoutGem2->addWidget(gemWidget);
        }

        const Engrave* pEngrave = m_pCharacter->getEngrave();
        if (pEngrave != nullptr)
        {
            m_pEngraveWidget = new EngraveWidget(this, pEngrave);
            ui->vLayoutEtc->addWidget(m_pEngraveWidget);
        }

        const Card* pCard = m_pCharacter->getCard();
        if (pCard != nullptr)
        {
            m_pCardWidget = new CardWidget(this, pCard);
            ui->vLayoutProfile->addWidget(m_pCardWidget);
        }

        const QList<Skill*>& skills = m_pCharacter->getSkills();
        for (const Skill* skill : skills)
        {
            SkillWidget* skillWidget = new SkillWidget(this, skill);
            m_skillWidgets.append(skillWidget);
            ui->vLayoutSkill->addWidget(skillWidget);
        }

        insertToDb();
    }
}

void CharacterSearch::reset()
{
    if (m_pCharacter != nullptr)
        delete m_pCharacter;
    m_pCharacter = new Character();

    if (m_pOthers != nullptr)
        delete m_pOthers;
    m_pOthers = nullptr;

    if (m_pProfileWidget != nullptr)
        delete m_pProfileWidget;
    m_pProfileWidget = nullptr;

    for (EquipWidget* equipWidget : m_equipWidgets)
        delete equipWidget;
    m_equipWidgets.clear();

    for (AccessoryWidget* accessoryWidget : m_accessoryWidgets)
        delete accessoryWidget;
    m_accessoryWidgets.clear();

    if (m_pStoneWidget != nullptr)
        delete m_pStoneWidget;
    m_pStoneWidget = nullptr;

    if (m_pBraceletWidget != nullptr)
        delete m_pBraceletWidget;
    m_pBraceletWidget = nullptr;

    for (GemWidget* gemWidget : m_gemWidgets)
        delete gemWidget;
    m_gemWidgets.clear();

    if (m_pEngraveWidget != nullptr)
        delete m_pEngraveWidget;
    m_pEngraveWidget = nullptr;

    if (m_pCardWidget != nullptr)
        delete m_pCardWidget;
    m_pCardWidget = nullptr;

    for (SkillWidget* skillWidget : m_skillWidgets)
        delete skillWidget;
    m_skillWidgets.clear();

    m_replyHandleStatus = 0x00;
}

void CharacterSearch::insertToDb()
{
    // Insert Character
    const Profile* pProfile = m_pCharacter->getProfile();
    if (pProfile == nullptr || pProfile->getItemLevel() < 1540)
        return;

    bsoncxx::document::value docValueCharacter = DocumentManager::buildDocumentCharacter(pProfile->getCharacterName(), pProfile->getClass(), pProfile->getItemLevel()).extract();
    m_pDbRequest->insertOrUpdateDocument(Collection::Character, docValueCharacter, "Name", pProfile->getCharacterName());

    // Insert SettingV2
    QList<Ability> abilities;
    for (int i = static_cast<int>(ItemType::목걸이); i <= static_cast<int>(ItemType::반지); i++)
    {
        ItemType type = static_cast<ItemType>(i);
        if (type == ItemType::목걸이)
        {
            const Accessory* pAcc = m_pCharacter->getAccessory(type);
            if (pAcc == nullptr)
                return;

            const auto& accAbilities = pAcc->getAbilities();
            abilities.append(accAbilities[0].first);
            abilities.append(accAbilities[1].first);
        }
        else
        {
            for (int j = 0; j < 2; j++)
            {
                const Accessory* pAcc = m_pCharacter->getAccessory(type, j);
                if (pAcc == nullptr)
                    return;

                const auto& accAbilities = pAcc->getAbilities();
                abilities.append(accAbilities[0].first);
            }
        }
    }
    QList<SetEffect> setEffects;
    for (int i = 0; i <= static_cast<int>(ItemType::어깨); i++)
    {
        ItemType type = static_cast<ItemType>(i);
        const Equip* pEquip = m_pCharacter->getEquip(type);
        if (pEquip == nullptr || pEquip->getGrade() < ItemGrade::유물)
            return;

        if (pEquip->getGrade() == ItemGrade::에스더 && type == ItemType::무기)
        {
            setEffects.append(m_pCharacter->getEquip(ItemType::장갑)->getSetEffect());
        }
        else
        {
            setEffects.append(pEquip->getSetEffect());
        }
    }
    QString settingCode = SettingCodeManager::generateSettingCode(abilities, setEffects, m_pCharacter->getEngrave()->getEngraves());

    bsoncxx::document::value docValueSettingV2 = DocumentManager::buildDocumentSettingV2(pProfile->getCharacterName(), pProfile->getClass(), settingCode).extract();
    m_pDbRequest->insertOrUpdateDocument(Collection::SettingV2, docValueSettingV2, "Name", pProfile->getCharacterName());
}

void CharacterSearch::handleCharacters(QNetworkReply* reply)
{
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    if (response.isNull())
    {
        CharacterSearch::getInstance()->ui->groupCharacter->hide();

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

                            QString slicedStr = effect.sliced(startIdx, endIdx - startIdx);
                            if (slicedStr.startsWith('['))
                            {
                                slicedStr = slicedStr.sliced(0, slicedStr.indexOf("]") + 1);
                            }
                            bracelet->addEffect(slicedStr);

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
        if (gem->getName().contains("멸화"))
            gem->setGemType(GemType::멸화);
        else if (gem->getName().contains("홍염"))
            gem->setGemType(GemType::홍염);
        slotToGem[slot] = gem;
    }

    QList<Gem*> gemList;
    for (const QJsonValue& effectValue : gemEffects)
    {
        const QJsonObject& effectObj = effectValue.toObject();
        int slot = effectObj.find("GemSlot")->toInt();
        QString effectStr = QString("%1").arg(effectObj.find("Name")->toString());

        Gem* gem = slotToGem[slot];
        gem->setEffect(effectStr);

        gemList.append(gem);
    }

    std::sort(gemList.begin(), gemList.end(), [&](Gem* a, Gem* b){
        return a->getLevel() > b->getLevel();
    });

    for (Gem* gem : gemList)
    {
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
