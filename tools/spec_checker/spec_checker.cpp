#include "spec_checker.h"
#include "ui_spec_checker.h"
#include "http_client/http_client.h"
#include "font/font_manager.h"
#include "tools/spec_checker/ui/spec_widget.h"
#include "game_data/character/character.h"
#include "game_data/profile/profile.h"
#include "game_data/item/equip.h"
#include "game_data/item/accessory.h"
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
#include <QMessageBox>

SpecChecker* SpecChecker::m_pSpecChecker = nullptr;
QRegularExpression SpecChecker::m_regExpHtmlTag("<[^>]*>");

SpecChecker::SpecChecker() :
    ui(new Ui::SpecChecker),
    m_replyHandleStatus(0x00),
    m_pNewCharacter(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("군장 검사");
    this->showMaximized();

    initNetworkManagers();
    initReplyHandlers();
    initConnect();
    initAlignments();
    initFonts();
}

SpecChecker::~SpecChecker()
{
    for (QNetworkAccessManager* pNetworkManager : m_networkManagers)
        delete pNetworkManager;
    m_networkManagers.clear();

    for (SpecWidget* pSpecWidget : m_specWidgets)
        delete pSpecWidget;
    m_specWidgets.clear();

    delete ui;
}

void SpecChecker::initNetworkManagers()
{
    for (int i = 0; i < MAX_NETWORK_MANAGER; i++)
    {
        m_networkManagers.append(new QNetworkAccessManager());
    }
}

void SpecChecker::initReplyHandlers()
{
    m_replyHandlers.append(handleProfiles);
    m_replyHandlers.append(handleEquipment);
    m_replyHandlers.append(handleSkills);
    m_replyHandlers.append(handleEngravings);
    m_replyHandlers.append(handleCards);
    m_replyHandlers.append(handleGems);
}

void SpecChecker::initConnect()
{
    for (int i = 0; i < MAX_NETWORK_MANAGER; i++)
    {
        connect(m_networkManagers[i], &QNetworkAccessManager::finished, m_replyHandlers[i]);
    }
    connect(ui->pbSearch, &QPushButton::pressed, this, &SpecChecker::sendRequests);
    connect(ui->leCharacterName, &QLineEdit::returnPressed, this, &SpecChecker::sendRequests);
    connect(ui->pbClear, &QPushButton::pressed, this, &SpecChecker::clearContents);
}

void SpecChecker::initAlignments()
{
    ui->vLayoutSpecChecker->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->hLayoutInput->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->vLayoutScrollArea->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->vLayoutContents->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void SpecChecker::initFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->pbSearch->setFont(nanumBold10);
    ui->pbClear->setFont(nanumBold10);
}

void SpecChecker::sendRequests()
{
    m_replyHandleStatus = 0x00;
    m_pNewCharacter = new Character();
    enableInput(false);

    HttpClient* pHttpClient = HttpClient::getInstance();
    QStringList params;
    params << ui->leCharacterName->text();
    ui->leCharacterName->clear();

    for (int i = 0; i < MAX_NETWORK_MANAGER; i++)
    {
        pHttpClient->sendGetRequest(m_networkManagers[i], static_cast<LostarkApi>(i + 1), 1, params);
    }
}

void SpecChecker::updateStatus(uint8_t bit)
{
    m_replyHandleStatus |= bit;
    if (m_replyHandleStatus == REPLY_HANDLE_FINISHED)
    {
        SpecWidget* pSpecWidget = new SpecWidget();
        pSpecWidget->setCharacter(m_pNewCharacter);
        m_specWidgets.append(pSpecWidget);
        ui->vLayoutContents->addWidget(pSpecWidget);
        enableInput(true);
    }
}

void SpecChecker::clearContents()
{
    for (SpecWidget* pSpecWidget : m_specWidgets)
        delete pSpecWidget;
    m_specWidgets.clear();

    m_pNewCharacter = nullptr;
}

void SpecChecker::enableInput(bool enable)
{
    ui->pbSearch->setEnabled(enable);
    ui->pbClear->setEnabled(enable);
}

void SpecChecker::handleProfiles(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();

    QJsonDocument profileDoc = QJsonDocument::fromJson(pReply->readAll());
    if (profileDoc.isNull())
    {
        QMessageBox msgBox;
        msgBox.setText("존재하지 않는 캐릭터입니다.");
        msgBox.exec();

        delete pSpecChecker->m_pNewCharacter;
        pSpecChecker->m_pNewCharacter = nullptr;
        pSpecChecker->enableInput(true);
        return;
    }

    const QJsonObject& profileObj = profileDoc.object();
    Profile* pProfile = new Profile();
    pProfile->setClass(strToClass(profileObj.find("CharacterClassName")->toString()));
    pProfile->setCharacterLevel(profileObj.find("CharacterLevel")->toInt());
    pProfile->setCharacterName(profileObj.find("CharacterName")->toString());
    pProfile->setServer(profileObj.find("ServerName")->toString());
    pProfile->setTitle(profileObj.find("Title")->toString());
    pProfile->setGuild(profileObj.find("GuildName")->toString());
    pProfile->setExpLevel(profileObj.find("ExpeditionLevel")->toInt());
    pProfile->setItemLevel(profileObj.find("ItemAvgLevel")->toString().remove(",").toDouble());

    const QJsonArray& abilities = profileObj.find("Stats")->toArray();
    for (const QJsonValue& value : abilities)
    {
        const QJsonObject& abilityObj = value.toObject();
        QString abilityStr = abilityObj.find("Type")->toString();
        Ability ability = strToAbility(abilityStr);
        if (ability != Ability::Size)
        {
            int abilityValue = abilityObj.find("Value")->toString().toInt();
            pProfile->addAbility(ability, abilityValue);
        }
    }

    pSpecChecker->m_pNewCharacter->setProfile(pProfile);
    pSpecChecker->updateStatus(1 << 0);
}

void SpecChecker::handleEquipment(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();
    uint8_t statusBit = 1 << 1;

    QJsonDocument equipDoc = QJsonDocument::fromJson(pReply->readAll());
    if (equipDoc.isNull())
    {
        pSpecChecker->updateStatus(statusBit);
        return;
    }

    const QJsonArray& equipments = equipDoc.array();
    for (const QJsonValue& equipment : equipments)
    {
        const QJsonObject& equipObj = equipment.toObject();
        ItemType itemType = strToItemType(equipObj.find("Type")->toString());
        if (itemType == ItemType::무기)
        {
            Equip* pEquip = new Equip(itemType);
            pEquip->setName(equipObj.find("Name")->toString());
            pEquip->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            // remove escape sequences by casting twice (to string -> to object)
            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QJsonObject& itemTitleObj = tooltipObj.find("Element_001")->toObject().find("value")->toObject();
            pEquip->setLevelTier(itemTitleObj.find("leftStr2")->toString().remove(m_regExpHtmlTag));
            pEquip->setQuality(itemTitleObj.find("qualityValue")->toInt());

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
                        pEquip->setSetEffect(strToSetEffect(setEffectLevel.sliced(0, 2)));
                        pEquip->setSetLevel(setEffectLevel.sliced(setEffectLevel.indexOf("Lv.") + 3, 1).toInt());
                    }
                }
                else if (type == "SingleTextBox")
                {
                    const QString& valueStr = elementObj.find("value")->toString();
                    if (valueStr.contains("엘라 부여 완료"))
                        pEquip->setElla(true);
                }
            }
            if (pEquip->getGrade() == ItemGrade::에스더)
                pEquip->setSetEffect(SetEffect::에스더);

            pSpecChecker->m_pNewCharacter->setEquip(itemType, pEquip);
        }
        else if (itemType >= ItemType::투구 && itemType <= ItemType::어깨)
        {
            Equip* pEquip = new Equip(itemType);
            pEquip->setName(equipObj.find("Name")->toString());
            pEquip->setGrade(strToItemGrade(equipObj.find("Grade")->toString()));

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            const QJsonObject& itemTitleObj = tooltipObj.find("Element_001")->toObject().find("value")->toObject();
            pEquip->setLevelTier(itemTitleObj.find("leftStr2")->toString().remove(m_regExpHtmlTag));
            pEquip->setQuality(itemTitleObj.find("qualityValue")->toInt());

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
                        pEquip->setSetEffect(strToSetEffect(setEffectLevel.sliced(0, 2)));
                        pEquip->setSetLevel(setEffectLevel.sliced(setEffectLevel.indexOf("Lv.") + 3, 1).toInt());
                    }
                }
            }

            pSpecChecker->m_pNewCharacter->setEquip(itemType, pEquip);
        }
        else if (itemType == ItemType::목걸이)
        {
            Accessory* pAccessory = new Accessory(itemType);
            pAccessory->setName(equipObj.find("Name")->toString());

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            pAccessory->setQuality(tooltipObj.find("Element_001")->toObject().find("value")->toObject().find("qualityValue")->toInt());

            pSpecChecker->m_pNewCharacter->setAccessory(itemType, pAccessory);
        }
        else if (itemType == ItemType::귀걸이 || itemType == ItemType::반지)
        {
            Accessory* pAccessory = new Accessory(itemType);
            pAccessory->setName(equipObj.find("Name")->toString());

            const QJsonObject& tooltipObj = QJsonDocument::fromJson(equipObj.find("Tooltip")->toString().toUtf8()).object();
            pAccessory->setQuality(tooltipObj.find("Element_001")->toObject().find("value")->toObject().find("qualityValue")->toInt());

            pSpecChecker->m_pNewCharacter->setAccessory(itemType, pAccessory);
        }
        else if (itemType == ItemType::팔찌)
        {
            Bracelet* pBracelet = new Bracelet();
            pBracelet->setName(equipObj.find("Name")->toString());

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
                            pBracelet->addEffect(slicedStr);

                            startIdx = effect.indexOf("@", startIdx);
                        }
                    }
                }
            }

            pSpecChecker->m_pNewCharacter->setBracelet(pBracelet);
        }
    }

    pSpecChecker->updateStatus(statusBit);
}

void SpecChecker::handleSkills(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();
    uint8_t statusBit = 1 << 2;

    QJsonDocument skillDoc = QJsonDocument::fromJson(pReply->readAll());
    if (skillDoc.isNull())
    {
        pSpecChecker->updateStatus(statusBit);
        return;
    }

    const QJsonArray& skills = skillDoc.array();
    for (const QJsonValue& skill : skills)
    {
        const QJsonObject skillObj = skill.toObject();
        int level = skillObj.find("Level")->toInt();
        bool bRune = !skillObj.find("Rune")->isNull();
        if (level > 1 || bRune)
        {
            Skill* pSkill = new Skill();
            pSkill->setName(skillObj.find("Name")->toString());
            pSkill->setLevel(level);

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
                    pSkill->addTripod(tripod);
                }
            }

            pSpecChecker->m_pNewCharacter->addSkill(pSkill);
        }
    }

    pSpecChecker->updateStatus(statusBit);
}

void SpecChecker::handleEngravings(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();
    uint8_t statusBit = 1 << 3;

    QJsonDocument engravingDoc = QJsonDocument::fromJson(pReply->readAll());
    if (engravingDoc.isNull())
    {
        pSpecChecker->updateStatus(statusBit);
        return;
    }

    Engrave* pEngrave = new Engrave();
    const QJsonArray& engravings = engravingDoc.object().find("Effects")->toArray();

    for (const QJsonValue& engraving : engravings)
    {
        const QJsonObject& engravingObj = engraving.toObject();
        QString nameLevel = engravingObj.find("Name")->toString();
        int endIndex = nameLevel.indexOf("Lv.") - 1;

        QString name = nameLevel.sliced(0, endIndex);
        int level = nameLevel.last(1).toInt();
        if (EngraveManager::getInstance()->isValidEngrave(name))
            pEngrave->addEngrave(name, level);
        else if (EngraveManager::getInstance()->isValidPenalty(name))
            pEngrave->addPenalty(name, level);
    }

    pSpecChecker->m_pNewCharacter->setEngrave(pEngrave);
    pSpecChecker->updateStatus(statusBit);
}

void SpecChecker::handleCards(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();
    uint8_t statusBit = 1 << 4;

    QJsonDocument cardDoc = QJsonDocument::fromJson(pReply->readAll());
    if (cardDoc.isNull())
    {
        pSpecChecker->updateStatus(statusBit);
        return;
    }

    const QJsonArray& cards = cardDoc.object().find("Effects")->toArray();
        for (const QJsonValue& card : cards)
        {
            Card* pCard = new Card();

            const QJsonArray& cardEffects = card.toObject().find("Items")->toArray();
            for (const QJsonValue& cardEffect : cardEffects)
            {
                const QJsonObject& effectObj = cardEffect.toObject();
                QString name = effectObj.find("Name")->toString();
                QString desc = effectObj.find("Description")->toString();
                pCard->addCardEffect(name, desc);
            }

            if (pCard->getEffectNames().size() != 0)
                pSpecChecker->m_pNewCharacter->addCard(pCard);
            else
                delete pCard;
        }

    pSpecChecker->updateStatus(statusBit);
}

void SpecChecker::handleGems(QNetworkReply* pReply)
{
    SpecChecker* pSpecChecker = SpecChecker::getInstance();
    uint8_t statusBit = 1 << 5;

    QJsonDocument gemDoc = QJsonDocument::fromJson(pReply->readAll());
    if (gemDoc.isNull())
    {
        pSpecChecker->updateStatus(statusBit);
        return ;
    }

    const QJsonArray& gems = gemDoc.object().find("Gems")->toArray();
    for (const QJsonValue& gem : gems)
    {
        const QJsonObject& gemObj = gem.toObject();

        Gem* pGem = new Gem();
        pGem->setName(gemObj.find("Name")->toString().remove(m_regExpHtmlTag));
        pGem->setLevel(gemObj.find("Level")->toInt());
        if (pGem->getName().contains("멸화"))
            pGem->setGemType(GemType::멸화);
        else if (pGem->getName().contains("홍염"))
            pGem->setGemType(GemType::홍염);

        pSpecChecker->m_pNewCharacter->addGem(pGem);
    }

    pSpecChecker->updateStatus(statusBit);
}

SpecChecker* SpecChecker::getInstance()
{
    if (m_pSpecChecker == nullptr)
        m_pSpecChecker = new SpecChecker();
    return m_pSpecChecker;
}

void SpecChecker::destroyInstance()
{
    if (m_pSpecChecker == nullptr)
        return;
    delete m_pSpecChecker;
    m_pSpecChecker = nullptr;
}
