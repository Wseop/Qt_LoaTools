#include "profile.h"
#include "ui_profile.h"
#include "character_list.h"
#include "card_set.h"
#include "game_data/profile/class.h"
#include "game_data/profile/engrave.h"
#include "http_client/json_builder.h"
#include "db/db_request.h"
#include "font/font_manager.h"

#include "ui/card_label.h"
#include "ui/equip_widget.h"
#include "ui/acc_widget.h"
#include "ui/abilitystone_widget.h"
#include "ui/bracelet_widget.h"
#include "ui/engrave_widget.h"
#include "ui/gem_widget.h"
#include "ui/skill_widget.h"

#include <QMessageBox>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPixmap>
#include <QLabel>
#include <QTabWidget>
#include <QFile>
#include <QDir>
#include <QThread>

Profile* Profile::m_pProfile = nullptr;

Profile::Profile() :
    ui(new Ui::Profile),
    m_pNetworkProfile(new QNetworkAccessManager()),
    m_pDBRequest(new DBRequest()),
    m_htmlTag("<[^>]*>")
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();

    initUI();
    initConnect();

    QThread* dbThread = new QThread();
    m_pDBRequest->moveToThread(dbThread);
    dbThread->start();
}

Profile::~Profile()
{
    delete ui;
    destroyInstance();
}

void Profile::profileRequest(QString name)
{
    ui->leName->setText(name);
    emit ui->pbSearch->pressed();
}

Profile *Profile::getInstance()
{
    if (m_pProfile == nullptr)
        m_pProfile = new Profile();

    return m_pProfile;
}

void Profile::destroyInstance()
{
    if (m_pProfile == nullptr)
        return ;

    delete m_pProfile;
    m_pProfile = nullptr;
}

void Profile::initUI()
{   
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);
    QFont fontNanumBold16 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 16);
    QFont fontNanumBold20 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 20);

    ui->leName->setFont(fontNanumBold10);
    ui->pbSearch->setFont(fontNanumBold10);
    ui->pbCharacterList->setFont(fontNanumBold10);
    ui->groupGuild->setFont(fontNanumBold10);
    ui->groupBoxItemLevel->setFont(fontNanumBold10);
    ui->groupCard->setFont(fontNanumBold10);
    ui->tabProfile->setFont(fontNanumBold10);
    ui->tabEquip->setFont(fontNanumBold10);
    ui->tabSkill->setFont(fontNanumBold10);

    ui->lbClassLevel->setFont(fontNanumBold12);
    ui->lbName->setFont(fontNanumBold12);

    ui->lbServer->setFont(fontNanumBold16);
    ui->lbGuild->setFont(fontNanumBold16);

    ui->lbItemLevel->setFont(fontNanumBold20);

    ui->vLayoutMain->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    ui->tabProfile->hide();
    ui->groupTitle->hide();

    ui->groupBoxSearch->setMaximumWidth(500);

    ui->tabEquip->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabSkill->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");

    // Title
    ui->vLayoutTitle->setAlignment(Qt::AlignTop);
    ui->lbServer->setStyleSheet("QLabel { color: #B178FF }");
    ui->lbGuild->setStyleSheet("QLabel { color: #00B700 }");
    ui->lbItemLevel->setStyleSheet("QLabel { color: #FF009B }");

    // Item
    ui->vLayoutEquip->setAlignment(Qt::AlignTop);
    ui->vLayoutAccessory->setAlignment(Qt::AlignTop);
    ui->vLayoutOther->setAlignment(Qt::AlignTop);
    ui->vLayoutGem->setAlignment(Qt::AlignTop);

    // Card
    ui->vLayoutCard->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void Profile::initConnect()
{
    connect(ui->pbSearch, SIGNAL(pressed()), this, SLOT(slotProfileRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(slotProfileRequest()));
    connect(ui->pbCharacterList, SIGNAL(pressed()), this, SLOT(slotShowCharacterList()));
    connect(m_pNetworkProfile, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotExtractProfile(QNetworkReply*)));

    connect(this, SIGNAL(sigUpdateTitle()), this, SLOT(slotUpdateTitle()));
    connect(this, SIGNAL(sigUpdateItem()), this, SLOT(slotUpdateItem()));
    connect(this, SIGNAL(sigUpdateGem()), this, SLOT(slotUpdateGem()));
    connect(this, SIGNAL(sigUpdateEngrave()), this, SLOT(slotUpdateEngrave()));
    connect(this, SIGNAL(sigUpdateCard()), this, SLOT(slotUpdateCard()));
    connect(this, SIGNAL(sigUpdateSkill()), this, SLOT(slotUpdateSkill()));
}

void Profile::parseTitle(const QString& profile)
{
    qsizetype startIndex, endIndex;

    // 직업 정보 추출
    QString classToken = "profile-character-info__img";
    startIndex = profile.indexOf(classToken);
    startIndex = profile.indexOf("alt=\"", startIndex) + 5;
    endIndex = profile.indexOf("\">", startIndex);
    m_pCharacter->setClass(profile.sliced(startIndex, endIndex - startIndex));

    // level 정보 추출
    classToken = "profile-character-info__lv";
    startIndex = profile.indexOf(classToken);
    startIndex = profile.indexOf("Lv.", startIndex);
    endIndex = profile.indexOf("</span>", startIndex);
    m_pCharacter->setLevel(profile.sliced(startIndex, endIndex - startIndex));

    // 서버 정보 추출
    classToken = "profile-character-info__server";
    startIndex = profile.indexOf(classToken);
    startIndex = profile.indexOf("@", startIndex);
    endIndex = profile.indexOf("\">", startIndex);
    m_pCharacter->setServer(profile.sliced(startIndex, endIndex - startIndex));

    parseCharacterList(profile);
    parseGuildName(profile);

    emit sigUpdateTitle();
}

void Profile::parseCharacterList(const QString &profile)
{
    qsizetype serverIndexStart = profile.indexOf("profile-character-list__server");
    qsizetype serverIndexEnd = 0;

    while (serverIndexStart != -1)
    {
        serverIndexStart = profile.indexOf("@", serverIndexStart);
        serverIndexEnd = profile.indexOf("</strong>", serverIndexStart);
        QString server = profile.sliced(serverIndexStart, serverIndexEnd - serverIndexStart);

        qsizetype startIndex = profile.indexOf("profile-character-list__char", serverIndexStart);
        qsizetype endIndex = 0;
        serverIndexStart = profile.indexOf("profile-character-list__server", startIndex);

        while (startIndex != -1)
        {
            startIndex = profile.indexOf("common/thumb", startIndex);
            startIndex = profile.indexOf("alt", startIndex) + 5;
            endIndex = profile.indexOf("\">", startIndex);
            QString cls = profile.sliced(startIndex, endIndex - startIndex);

            startIndex = profile.indexOf("<span>", startIndex) + 6;
            endIndex = profile.indexOf("</span>", startIndex);
            QString name = profile.sliced(startIndex, endIndex - startIndex);

            m_pCharacterList->addCharacter(server, name, cls);

            startIndex = profile.indexOf("common/thumb", startIndex);
            if (serverIndexStart != -1 && startIndex > serverIndexStart)
                break;
        }
    }
}

void Profile::parseGuildName(const QString &profile)
{
    qsizetype indexStart = profile.indexOf("game-info__guild");
    indexStart = profile.indexOf("<span>", indexStart) + 1;
    indexStart = profile.indexOf("<span>", indexStart) + 6;
    if (profile[indexStart] == '<')
        indexStart = profile.indexOf(">", indexStart) + 1;
    qsizetype indexEnd = profile.indexOf("</span>", indexStart);

    m_pCharacter->setGuild(profile.sliced(indexStart, indexEnd - indexStart));
}

void Profile::parseItem()
{
    const QJsonObject& obj = m_pProfileObj->find("Equip")->toObject();

    const QStringList& objKeys = obj.keys();
    // gem(보석 아이템)만 구분하여 item key 추출
    QStringList keys;
    QStringList gemKeys;
    for (const QString& key : objKeys)
    {
        if (key.contains("Gem"))
            gemKeys << key;
        else
            keys << key;
    }

    // item parsing
    for (const QString& key : keys)
    {
        const QJsonObject& itemObj = obj.find(key)->toObject();

        if (key.endsWith("0" + QString::number(static_cast<int>(ItemPart::WEAPON))) ||
            key.endsWith("0" + QString::number(static_cast<int>(ItemPart::HEAD))) ||
            key.endsWith("0" + QString::number(static_cast<int>(ItemPart::TOP))) ||
            key.endsWith("0" + QString::number(static_cast<int>(ItemPart::BOTTOM))) ||
            key.endsWith("0" + QString::number(static_cast<int>(ItemPart::HAND))) ||
            key.endsWith("0" + QString::number(static_cast<int>(ItemPart::SHOULDER))))
        {
            ItemPart part = static_cast<ItemPart>(key.last(3).toInt());
            parseEquip(itemObj, part);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(ItemPart::NECK))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(ItemPart::EAR1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(ItemPart::EAR2))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(ItemPart::RING1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(ItemPart::RING2))))
        {
            ItemPart part = static_cast<ItemPart>(key.last(3).toInt());
            parseAccessory(itemObj, part);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(ItemPart::STONE))))
        {
            parseStone(itemObj);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(ItemPart::BRACELET))))
        {
            parseBracelet(itemObj);
        }
    }
    emit sigUpdateItem();

    // gem parsing
    for (const QString& key : gemKeys)
    {
        const QJsonObject& gemObj = obj.find(key)->toObject();
        parseGem(gemObj);
    }
    emit sigUpdateGem();
}

void Profile::parseEquip(const QJsonObject &equipObj, ItemPart part)
{
    Equip* equip = new Equip(part);
    const QStringList& elementKeys = equipObj.keys();

    int itemLevel = 0;
    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = equipObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(m_htmlTag);
            equip->setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            ItemGrade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString levelTier = valueObj.find("leftStr2")->toString().remove(m_htmlTag);
            int quality = valueObj.find("qualityValue")->toInt();
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            equip->setGrade(grade);
            equip->setLevelTier(levelTier);
            equip->setQuality(quality);
            equip->setIconPath(iconPath);

            qsizetype indexStart = levelTier.indexOf("아이템 레벨") + 7;
            qsizetype indexEnd = levelTier.indexOf("(");
            if (indexEnd == -1)
                itemLevel = levelTier.sliced(indexStart).toInt();
            else
                itemLevel = levelTier.sliced(indexStart, indexEnd - indexStart).toInt();
        }
        else if (type == "ItemPartBox")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            if (valueObj.find("Element_000")->toString().contains("세트 효과 레벨"))
            {
                QString setLevel = valueObj.find("Element_001")->toString().remove(m_htmlTag);
                equip->setSetLevel(setLevel);
            }
        }
    }

    m_pCharacter->setItem(static_cast<Item*>(equip));
    m_pCharacter->addItemLevel(itemLevel);

    if (equip->getSetLevel() != "")
    {
        if (part != ItemPart::WEAPON || (part == ItemPart::WEAPON && equip->getGrade() != ItemGrade::ESTHER))
        {
            m_pCharacter->addSetEffect(equip->getSetLevel().sliced(0, 2), static_cast<int>(part));
        }

        const Equip* weapon = static_cast<const Equip*>(m_pCharacter->getItemByPart(ItemPart::WEAPON));
        if (weapon == nullptr)
            return;

        if (part == ItemPart::HAND && weapon->getGrade() == ItemGrade::ESTHER)
        {
            m_pCharacter->addSetEffect(equip->getSetLevel().sliced(0, 2), static_cast<int>(ItemPart::WEAPON));
        }
    }
}

void Profile::parseAccessory(const QJsonObject &accObj, ItemPart part)
{
    Accessory* acc = new Accessory(part);
    const QStringList& elementKeys = accObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = accObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(m_htmlTag);
            acc->setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            ItemGrade grade = extractGrade(valueObj.find("leftStr0")->toString());
            int quality = valueObj.find("qualityValue")->toInt();
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            acc->setGrade(grade);
            acc->setQuality(quality);
            acc->setIconPath(iconPath);
        }
        else if (type == "ItemPartBox")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            if (valueObj.find("Element_000")->toString().contains("추가 효과"))
            {
                QString ability = valueObj.find("Element_001")->toString();
                acc->setAbility(ability);
            }
        }
        else if (type == "IndentStringGroup")
        {
            const QJsonObject& valueObj = element.find("value")->toObject().find("Element_000")->toObject();

            if (valueObj.find("topStr")->toString().contains("각인"))
            {
                const QJsonObject& engraves = valueObj.find("contentStr")->toObject();
                const QStringList& engraveKeys = engraves.keys();

                for (const QString& engraveKey : engraveKeys)
                {
                    const QJsonObject& engraveObj = engraves.find(engraveKey)->toObject();

                    QString engrave = engraveObj.find("contentStr")->toString();
                    engrave.replace("#FFFFAC", "#B9B919");
                    if (engrave.contains("감소"))
                    {
                        acc->setPenalty(engrave);
                        extractEngraveValue(1, engrave);
                    }
                    else
                    {
                        acc->addEngrave(engrave);
                        extractEngraveValue(0, engrave);
                    }
                }
            }
        }
    }

    m_pCharacter->setItem(static_cast<Item*>(acc));
}

void Profile::parseStone(const QJsonObject &stoneObj)
{
    AbilityStone* stone = new AbilityStone();
    const QStringList& elementKeys = stoneObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = stoneObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(m_htmlTag);
            stone->setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            ItemGrade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            stone->setGrade(grade);
            stone->setIconPath(iconPath);
        }
        else if (type == "IndentStringGroup")
        {
            const QJsonObject& valueObj = element.find("value")->toObject().find("Element_000")->toObject();

            if (valueObj.find("topStr")->toString().contains("각인"))
            {
                const QJsonObject& engraves = valueObj.find("contentStr")->toObject();
                const QStringList& engraveKeys = engraves.keys();

                for (const QString& engraveKey : engraveKeys)
                {
                    const QJsonObject& engraveObj = engraves.find(engraveKey)->toObject();

                    QString engrave = engraveObj.find("contentStr")->toString();
                    engrave.replace("#FFFFAC", "#B9B919");
                    if (engrave.contains("감소"))
                    {
                        stone->setPenalty(engrave);
                        extractEngraveValue(1, engrave);
                    }
                    else
                    {
                        stone->addEngrave(engrave);
                        extractEngraveValue(0, engrave);
                    }
                }
            }
        }
    }

    m_pCharacter->setItem(static_cast<Item*>(stone));
}

void Profile::parseBracelet(const QJsonObject &braceletObj)
{
    Bracelet* bracelet = new Bracelet();
    const QStringList& elementKeys = braceletObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = braceletObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(m_htmlTag);
            bracelet->setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            ItemGrade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            bracelet->setGrade(grade);
            bracelet->setIconPath(iconPath);
        }
        else if (type == "ItemPartBox")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            if (valueObj.find("Element_000")->toString().contains("팔찌 효과"))
            {
                QString effect = valueObj.find("Element_001")->toString();
                effect = effect.remove(QRegularExpression("<img[^>]*>"));
                effect = effect.replace("</img>", "-");
                effect = effect.replace("#F9F7D0", "#B9B919", Qt::CaseInsensitive);
                effect = effect.replace("#99FF99", "#0ADC64", Qt::CaseInsensitive);
                bracelet->setEffect(effect);
            }
        }
    }

    m_pCharacter->setItem(static_cast<Item*>(bracelet));
}

void Profile::parseGem(const QJsonObject &gemObj)
{
    Gem gem;
    const QStringList& elementKeys = gemObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = gemObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(m_htmlTag);
            gem.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            ItemGrade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            int level = valueObj.find("slotData")->toObject().find("rtString")->toString().remove("Lv.").toInt();
            gem.setGrade(grade);
            gem.setIconPath(iconPath);
            gem.setLevel(level);
        }
        else if (type == "ItemPartBox")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            if (valueObj.find("Element_000")->toString().contains("효과"))
            {
                QString effect = valueObj.find("Element_001")->toString();
                effect = effect.replace("#FFD200", "#B9B919", Qt::CaseInsensitive);
                gem.setEffect(effect);
            }
        }
    }

    m_pCharacter->addGem(gem);
}

void Profile::parseEngrave()
{
    const QJsonObject& obj = m_pProfileObj->find("Engrave")->toObject();
    const QStringList& objKeys = obj.keys();

    // 장착 각인 parsing
    for (const QString& objKey : objKeys)
    {
        const QJsonObject& engraveObj = obj.find(objKey)->toObject();
        const QStringList& elementKeys = engraveObj.keys();

        QString name;
        int value = 0;
        for (const QString& elementKey : elementKeys)
        {
            const QJsonObject& element = engraveObj.find(elementKey)->toObject();
            QString type = element.find("type")->toString();

            if (type == "NameTagBox")
            {
                name = element.find("value")->toString();
            }
            else if (type == "EngraveSkillTitle")
            {
                value = element.find("value")->toObject().find("leftText")->toString()
                        .remove(m_htmlTag).remove("각인 활성 포인트 +").toInt();
            }
        }
        m_pCharacter->addEngrave(name, value);
    }

    emit sigUpdateEngrave();
}

void Profile::parseCard()
{
    const QJsonObject& cardSetObj = m_pProfileObj->find("CardSet")->toObject();
    QStringList keys = cardSetObj.keys();

    CardSet cardSet;
    for (const QString& key : keys)
    {
        const QJsonObject& obj = cardSetObj.find(key)->toObject();
        for (int i = 0; i < 6; i++)
        {
            QString effectKey = QString("Effect_00%1").arg(i);
            auto iter = obj.find(effectKey);
            if (iter == obj.end())
                break;

            const QJsonObject& content = iter->toObject();
            cardSet.addTitle(content.find("title")->toString());
            cardSet.addDesc(content.find("desc")->toString());
        }
    }

    m_pCharacter->setCardSet(cardSet);
    emit sigUpdateCard();
}

void Profile::parseSkill()
{
    const QJsonObject& skillObj = m_pProfileObj->find("Skill")->toObject();
    QStringList keys = skillObj.keys();

    for (const QString& key : keys)
    {
        const QJsonObject& obj = skillObj.find(key)->toObject();
        QStringList elements = obj.keys();

        Skill skill;
        bool bRuneExist = false;
        for (const QString& element : elements)
        {
            const QJsonObject& data = obj.find(element)->toObject();
            QString type = data.find("type")->toString();

            if (type == "NameTagBox")
            {
                QString name = data.find("value")->toString();
                skill.setName(name);
            }
            else if (type == "CommonSkillTitle")
            {
                const QJsonObject& valueObj = data.find("value")->toObject();
                QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
                skill.setIconPath(iconPath);
            }
            else if (type == "SingleTextBox")
            {
                QString levelStr = data.find("value")->toString();
                if (levelStr.startsWith("스킬 레벨"))
                {
                    int level = levelStr.remove("스킬 레벨 ").remove( "(최대)").toInt();
                    skill.setLevel(level);
                }
            }
            else if (type == "TripodSkillCustom")
            {
                const QJsonObject& tripods = data.find("value")->toObject();

                for (int i = 0; i < 3; i++)
                {
                    QString tripodKey = QString("Element_00%1").arg(i);
                    const QJsonObject& tripodObj = tripods.find(tripodKey)->toObject();

                    if (tripodObj.find("lock")->toBool())
                        break;

                    Tripod tripod;
                    tripod.name = tripodObj.find("name")->toString().remove(m_htmlTag);
                    tripod.level = tripodObj.find("tier")->toString().remove(m_htmlTag).remove("레벨 ");
                    skill.addTripod(tripod);
                }
            }
            else if (type == "ItemPartBox")
            {
                const QJsonObject& value = data.find("value")->toObject();

                if (value.find("Element_000")->toString().contains("룬"))
                {
                    bRuneExist = true;
                    QString element001 = value.find("Element_001")->toString();
                    int indexColor = element001.indexOf("#");
                    QString color = element001.sliced(indexColor, 7);
                    QString name = element001.remove(m_htmlTag).sliced(1, 2);

                    Rune* rune = new Rune();
                    rune->setName(name);
                    rune->setGrade(getGradeByColor(color));
                    skill.setRune(rune);
                }
            }
        }
        // 스킬레벨이 2이상이거나 룬이 착용되어 있는 경우만 추가
        if (skill.getLevel() >= 2 || bRuneExist)
        {
            m_pCharacter->addSkill(skill);
        }
    }

    emit sigUpdateSkill();
}

void Profile::updateDB()
{
    if (m_pCharacter->getItemLevel() >= (double)1490)
    {
        // 미장착 슬롯 체크
        const Equip* equip = nullptr;
        for (int part = static_cast<int>(ItemPart::WEAPON); part <= static_cast<int>(ItemPart::SHOULDER); part++)
        {
            equip = static_cast<const Equip*>(m_pCharacter->getItemByPart(static_cast<ItemPart>(part)));
            if (equip == nullptr)
                return;
        }

        // Name, Class
        QString name = m_pCharacter->getName();
        QString cls = Class::kStrToEstr(m_pCharacter->getClass());

        // Abilities
        QStringList abilities;
        QString abilityStr;
        const Accessory* accessory = nullptr;
        accessory = static_cast<const Accessory*>(m_pCharacter->getItemByPart(ItemPart::NECK));
        if (accessory == nullptr)
            return;
        abilityStr = accessory->getAbility();
        abilities << abilityStr.sliced(0, 2);
        abilities << abilityStr.sliced(abilityStr.indexOf("<BR>") + 4, 2);
        for (int part = static_cast<int>(ItemPart::EAR1); part <= static_cast<int>(ItemPart::RING2); part++)
        {
            accessory = static_cast<const Accessory*>(m_pCharacter->getItemByPart(static_cast<ItemPart>(part)));
            if (accessory == nullptr)
                return;
            abilityStr = accessory->getAbility();
            abilities << abilityStr.sliced(0, 2);
        }

        // Engraves


        const QMap<QString, int>& engraveValues = m_pCharacter->getEngraveValues();
        QStringList engraveNames;
        QList<int> engraveLevels;
        engraveNames = Engrave::getInstance()->extractActiveEngraves(engraveValues);
        for (const QString& engraveName : engraveNames)
        {
            engraveLevels.append(engraveValues[engraveName] / 5);
        }

        // DB update - Character
        QJsonObject characterObj = JsonBuilder::buildCharacter(name, cls, m_pCharacter->getItemLevel()).object();
        emit m_pDBRequest->insertDocument(Collection::Character, characterObj);

        // DB update - Setting
        QJsonObject settingObj = JsonBuilder::buildSetting(name, cls, abilities, engraveNames, engraveLevels, m_pCharacter->getSetEffects()).object();
        emit m_pDBRequest->insertDocument(Collection::Setting, settingObj);
    }
}

ItemGrade Profile::extractGrade(QString str)
{
    if (str.contains("고급"))
        return ItemGrade::UNCOMMON;
    else if (str.contains("희귀"))
        return ItemGrade::RARE;
    else if (str.contains("영웅"))
        return ItemGrade::EPIC;
    else if (str.contains("전설"))
        return ItemGrade::LEGEND;
    else if (str.contains("유물"))
        return ItemGrade::RELIC;
    else if (str.contains("고대"))
        return ItemGrade::ANCIENT;
    else if (str.contains("에스더"))
        return ItemGrade::ESTHER;
    else
        return ItemGrade::NONE;
}

void Profile::extractEngraveValue(int type, QString engrave)
{
    int indexStart, indexEnd, engraveValue;
    QString engraveName;

    // 각인명 추출
    indexStart = engrave.indexOf(">") + 1;
    indexEnd = engrave.indexOf("</FONT>");
    engraveName = engrave.sliced(indexStart, indexEnd - indexStart);

    // 각인값 추출
    indexStart = engrave.indexOf("+") + 1;
    indexEnd = engrave.indexOf("<BR>");
    engraveValue = engrave.sliced(indexStart, indexEnd - indexStart).toInt();

    if (type == 0)
        m_pCharacter->addEngrave(engraveName, engraveValue);
    else
        m_pCharacter->addPenalty(engraveName, engraveValue);
}

ItemGrade Profile::getGradeByColor(QString color)
{
    if (color.compare("#8DF901", Qt::CaseInsensitive) == 0)
        return ItemGrade::UNCOMMON;
    else if (color.compare("#00B0FA", Qt::CaseInsensitive) == 0)
        return ItemGrade::RARE;
    else if (color.compare("#CE43FC", Qt::CaseInsensitive) == 0)
        return ItemGrade::EPIC;
    else if (color.compare("#F99200", Qt::CaseInsensitive) == 0)
        return ItemGrade::LEGEND;
    else if (color.compare("#FA5D00", Qt::CaseInsensitive) == 0)
        return ItemGrade::RELIC;
    else if (color.compare("#E3C7A1", Qt::CaseInsensitive) == 0)
        return ItemGrade::ANCIENT;
    else if (color.compare("#3CF2E6", Qt::CaseInsensitive) == 0)
        return ItemGrade::ESTHER;
    else
        return ItemGrade::NONE;
}

void Profile::clearAll()
{
    // 이전 캐릭터 정보 초기화
    if (m_pCharacter != nullptr)
    {
        delete m_pCharacter;
        m_pCharacter = nullptr;
    }
    if (m_pCharacterList != nullptr)
    {
        delete m_pCharacterList;
        m_pCharacterList = nullptr;
    }

    for (EquipWidget* equipWidget : m_equipWidgets)
    {
        ui->vLayoutEquip->removeWidget(equipWidget);
        delete equipWidget;
    }
    m_equipWidgets.clear();

    for (AccWidget* accWidget : m_accWidgets)
    {
        ui->vLayoutAccessory->removeWidget(accWidget);
        delete accWidget;
    }
    m_accWidgets.clear();

    if (m_pStoneWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(m_pStoneWidget);
        delete m_pStoneWidget;
        m_pStoneWidget = nullptr;
    }

    if (m_pBraceletWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(m_pBraceletWidget);
        delete m_pBraceletWidget;
        m_pBraceletWidget = nullptr;
    }

    if (m_pEngraveWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(m_pEngraveWidget);
        delete m_pEngraveWidget;
        m_pEngraveWidget = nullptr;
    }

    for (GemWidget* gemWidget : m_gemWidgets)
    {
        ui->vLayoutGem->removeWidget(gemWidget);
        delete gemWidget;
    }
    m_gemWidgets.clear();

    for (CardLabel* cardLabel : m_cardLabels)
    {
        ui->vLayoutCard->removeWidget(cardLabel);
        delete cardLabel;
    }
    m_cardLabels.clear();

    for (SkillWidget* skillWidget : m_skillWidgets)
    {
        ui->vLayoutSkill->removeWidget(skillWidget);
        delete skillWidget;
    }
    m_skillWidgets.clear();
}

void Profile::slotProfileRequest()
{
    // 입력받은 캐릭터명으로 전투정보실에 request를 보냄
    QString name = ui->leName->text();
    QString url = PATH_PROFILE + "/" + name;
    QNetworkRequest request((QUrl(url)));
    m_pNetworkProfile->get(request);
}

// 응답 결과로부터 Profile(Json형식) 부분 추출
void Profile::slotExtractProfile(QNetworkReply* reply)
{
    const QString profileStart = "$.Profile = ";
    const QString profileEnd = "};";

    QString responseData = reply->readAll();
    qsizetype profileIndex = responseData.indexOf(profileStart);
    qsizetype profileSize = 0;

    if (ui->leName->text() == "")
        return;

    // 존재하지 않는 캐릭터인 경우 알람 popup 후 종료
    if (profileIndex == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("존재하지 않는 캐릭터입니다.");
        msgBox.exec();
        ui->leName->clear();
        return;
    }
    else
    {
        if (ui->tabProfile->isHidden())
            ui->tabProfile->show();
        if (ui->groupTitle->isHidden())
            ui->groupTitle->show();

        // Profile 추출
        profileIndex += profileStart.size();
        profileSize = responseData.indexOf(profileEnd) - profileIndex + 1;
        const QString& profile = responseData.sliced(profileIndex, profileSize);
        if (m_pProfileObj != nullptr)
            delete m_pProfileObj;
        m_pProfileObj = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());

        // 기존 데이터 초기화
        clearAll();
        m_pCharacter = new Character();
        m_pCharacter->setName(ui->leName->text());
        ui->leName->clear();
        m_pCharacterList = new CharacterList(this);

        // 추출 결과 parsing
        parseItem();
        parseEngrave();
        parseCard();
        parseSkill();
        parseTitle(responseData);

        updateDB();
    }
}

void Profile::slotShowCharacterList()
{
    this->setDisabled(true);
    m_pCharacterList->show();
}

void Profile::slotUpdateTitle()
{
    ui->lbClassLevel->setText(QString("%1 %2").arg(m_pCharacter->getClass(), m_pCharacter->getLevel()));
    ui->lbName->setText(m_pCharacter->getName());
    ui->lbServer->setText(m_pCharacter->getServer());
    ui->lbGuild->setText(m_pCharacter->getGuild());
    ui->lbItemLevel->setText(QString("%1").arg(m_pCharacter->getItemLevel()));
}

void Profile::slotUpdateItem()
{
    const int INDEX_EQUIP = static_cast<int>(ItemPart::WEAPON);
    const int INDEX_ACCESSORY = static_cast<int>(ItemPart::NECK);
    const int INDEX_STONE = static_cast<int>(ItemPart::STONE);

    ItemPart part;
    for (int i = INDEX_EQUIP; i < INDEX_ACCESSORY; i++)
    {
        part = static_cast<ItemPart>(i);
        const Equip* equip = static_cast<const Equip*>(m_pCharacter->getItemByPart(part));
        if (equip == nullptr)
            continue;
        EquipWidget* equipWidget = new EquipWidget(equip, PATH_CDN);
        ui->vLayoutEquip->addWidget(equipWidget);
        m_equipWidgets.append(equipWidget);
    }

    for (int i = INDEX_ACCESSORY; i < INDEX_STONE; i++)
    {
        part = static_cast<ItemPart>(i);
        const Accessory* acc = static_cast<const Accessory*>(m_pCharacter->getItemByPart(part));
        if (acc == nullptr)
            continue;
        AccWidget* accWidget = new AccWidget(acc, PATH_CDN);
        ui->vLayoutAccessory->addWidget(accWidget);
        m_accWidgets.append(accWidget);
    }

    part = ItemPart::STONE;
    const AbilityStone* stone = static_cast<const AbilityStone*>(m_pCharacter->getItemByPart(part));
    if (stone != nullptr)
    {
        AbilityStoneWidget* stoneWidget = new AbilityStoneWidget(stone, PATH_CDN);
        ui->vLayoutOther->addWidget(stoneWidget);
        m_pStoneWidget = stoneWidget;
    }

    part = ItemPart::BRACELET;
    const Bracelet* bracelet = static_cast<const Bracelet*>(m_pCharacter->getItemByPart(part));
    if (bracelet != nullptr)
    {
        BraceletWidget* braceletWidget = new BraceletWidget(bracelet, PATH_CDN);
        ui->vLayoutOther->addWidget(braceletWidget);
        m_pBraceletWidget = braceletWidget;
    }
}

void Profile::slotUpdateGem()
{
    const QList<Gem>& gems = m_pCharacter->getGems();

    for (const Gem& gem : gems)
    {
        GemWidget* gemWidget = new GemWidget(&gem, PATH_CDN);
        ui->vLayoutGem->addWidget(gemWidget);
        m_gemWidgets.append(gemWidget);
    }
}

void Profile::slotUpdateEngrave()
{
    EngraveWidget* engraveWidget = new EngraveWidget(m_pCharacter->getEngraveValues(), m_pCharacter->getPenaltyValues());
    ui->vLayoutOther->addWidget(engraveWidget);
    m_pEngraveWidget = engraveWidget;
}

void Profile::slotUpdateCard()
{
    const CardSet& cardSet = m_pCharacter->getCardSet();

    for (int i = 0; i < cardSet.count(); i++)
    {
        CardLabel* cardLabel = new CardLabel(cardSet.getTitle(i), cardSet.getDesc(i));
        ui->vLayoutCard->addWidget(cardLabel);
        m_cardLabels.append(cardLabel);
    }
}

void Profile::slotUpdateSkill()
{
    const QList<Skill>& skills = m_pCharacter->getSkills();

    for (const Skill& skill : skills)
    {
        SkillWidget* skillWidget = new SkillWidget(&skill, PATH_CDN);
        ui->vLayoutSkill->addWidget(skillWidget);
        m_skillWidgets.append(skillWidget);
    }
}
