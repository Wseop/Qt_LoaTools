#include "profile.h"
#include "ui_profile.h"
#include "character_list.h"
#include "card_set.h"

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

Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile),
    mNetworkProfile(new QNetworkAccessManager()),
    mHtmlTag("<[^>]*>")
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();

    initUI();
    initConnect();
}

Profile::~Profile()
{
    delete ui;
}

void Profile::profileRequest(QString name)
{
    ui->leName->setText(name);
    emit ui->pbSearch->pressed();
}

void Profile::initUI()
{   
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
    connect(mNetworkProfile, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotExtractProfile(QNetworkReply*)));

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
    mCharacter->setClass(profile.sliced(startIndex, endIndex - startIndex));

    // level 정보 추출
    classToken = "profile-character-info__lv";
    startIndex = profile.indexOf(classToken);
    startIndex = profile.indexOf("Lv.", startIndex);
    endIndex = profile.indexOf("</span>", startIndex);
    mCharacter->setLevel(profile.sliced(startIndex, endIndex - startIndex));

    // 서버 정보 추출
    classToken = "profile-character-info__server";
    startIndex = profile.indexOf(classToken);
    startIndex = profile.indexOf("@", startIndex);
    endIndex = profile.indexOf("\">", startIndex);
    mCharacter->setServer(profile.sliced(startIndex, endIndex - startIndex));

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

            mCharacterList->addCharacter(server, name, cls);

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

    mCharacter->setGuild(profile.sliced(indexStart, indexEnd - indexStart));
}

void Profile::parseItem()
{
    const QJsonObject& obj = mProfile->find("Equip")->toObject();

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

        if (key.endsWith("0" + QString::number(static_cast<int>(Part::WEAPON))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HEAD))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::TOP))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::BOTTOM))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HAND))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::SHOULDER))))
        {
            Part part = static_cast<Part>(key.last(3).toInt());
            parseEquip(itemObj, part);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::NECK))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR2))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING2))))
        {
            Part part = static_cast<Part>(key.last(3).toInt());
            parseAccessory(itemObj, part);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::STONE))))
        {
            parseStone(itemObj);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::BRACELET))))
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

void Profile::parseEquip(const QJsonObject &equipObj, Part part)
{
    Equip equip(part);
    const QStringList& elementKeys = equipObj.keys();

    int itemLevel = 0;
    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = equipObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(mHtmlTag);
            equip.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            Grade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString levelTier = valueObj.find("leftStr2")->toString().remove(mHtmlTag);
            int quality = valueObj.find("qualityValue")->toInt();
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            equip.setGrade(grade);
            equip.setLevelTier(levelTier);
            equip.setQuality(quality);
            equip.setIconPath(iconPath);

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
                QString setLevel = valueObj.find("Element_001")->toString().remove(mHtmlTag);
                equip.setSetLevel(setLevel);
            }
        }
    }

    mCharacter->setItem(static_cast<const Item&>(equip));
    mCharacter->addItemLevel(itemLevel);
}

void Profile::parseAccessory(const QJsonObject &accObj, Part part)
{
    Accessory acc(part);
    const QStringList& elementKeys = accObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = accObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(mHtmlTag);
            acc.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            Grade grade = extractGrade(valueObj.find("leftStr0")->toString());
            int quality = valueObj.find("qualityValue")->toInt();
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            acc.setGrade(grade);
            acc.setQuality(quality);
            acc.setIconPath(iconPath);
        }
        else if (type == "ItemPartBox")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            if (valueObj.find("Element_000")->toString().contains("추가 효과"))
            {
                QString attr = valueObj.find("Element_001")->toString();
                acc.setAttr(attr);
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
                        acc.setPenalty(engrave);
                        extractEngraveValue(1, engrave);
                    }
                    else
                    {
                        acc.addEngrave(engrave);
                        extractEngraveValue(0, engrave);
                    }
                }
            }
        }
    }

    mCharacter->setItem(static_cast<const Item&>(acc));
}

void Profile::parseStone(const QJsonObject &stoneObj)
{
    AbilityStone stone;
    const QStringList& elementKeys = stoneObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = stoneObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(mHtmlTag);
            stone.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            Grade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            stone.setGrade(grade);
            stone.setIconPath(iconPath);
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
                        stone.setPenalty(engrave);
                        extractEngraveValue(1, engrave);
                    }
                    else
                    {
                        stone.addEngrave(engrave);
                        extractEngraveValue(0, engrave);
                    }
                }
            }
        }
    }

    mCharacter->setItem(static_cast<const Item&>(stone));
}

void Profile::parseBracelet(const QJsonObject &braceletObj)
{
    Bracelet bracelet;
    const QStringList& elementKeys = braceletObj.keys();

    for (const QString& elementKey : elementKeys)
    {
        const QJsonObject& element = braceletObj.find(elementKey)->toObject();
        QString type = element.find("type")->toString();

        if (type == "NameTagBox")
        {
            QString name = element.find("value")->toString().remove(mHtmlTag);
            bracelet.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            Grade grade = extractGrade(valueObj.find("leftStr0")->toString());
            QString iconPath = "/" + valueObj.find("slotData")->toObject().find("iconPath")->toString();
            bracelet.setGrade(grade);
            bracelet.setIconPath(iconPath);
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
                bracelet.setEffect(effect);
            }
        }
    }

    mCharacter->setItem(static_cast<const Item&>(bracelet));
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
            QString name = element.find("value")->toString().remove(mHtmlTag);
            gem.setName(name);
        }
        else if (type == "ItemTitle")
        {
            const QJsonObject& valueObj = element.find("value")->toObject();

            Grade grade = extractGrade(valueObj.find("leftStr0")->toString());
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

    mCharacter->addGem(gem);
}

void Profile::parseEngrave()
{
    const QJsonObject& obj = mProfile->find("Engrave")->toObject();
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
                        .remove(mHtmlTag).remove("각인 활성 포인트 +").toInt();
            }
        }
        mCharacter->addEngrave(name, value);
    }

    emit sigUpdateEngrave();
}

void Profile::parseCard()
{
    const QJsonObject& cardSetObj = mProfile->find("CardSet")->toObject();
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

    mCharacter->setCardSet(cardSet);
    emit sigUpdateCard();
}

void Profile::parseSkill()
{
    const QJsonObject& skill = mProfile->find("Skill")->toObject();
    QStringList keys = skill.keys();

    for (const QString& key : keys)
    {
        const QJsonObject& obj = skill.find(key)->toObject();
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
                    tripod.name = tripodObj.find("name")->toString().remove(mHtmlTag);
                    tripod.level = tripodObj.find("tier")->toString().remove(mHtmlTag).remove("레벨 ");
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
                    QString name = element001.remove(mHtmlTag).sliced(1, 2);

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
            mCharacter->addSkill(skill);
        }
    }

    emit sigUpdateSkill();
}

Grade Profile::extractGrade(QString str)
{
    if (str.contains("고급"))
        return Grade::UNCOMMON;
    else if (str.contains("희귀"))
        return Grade::RARE;
    else if (str.contains("영웅"))
        return Grade::EPIC;
    else if (str.contains("전설"))
        return Grade::LEGEND;
    else if (str.contains("유물"))
        return Grade::RELIC;
    else if (str.contains("고대"))
        return Grade::ANCIENT;
    else if (str.contains("에스더"))
        return Grade::ESTHER;
    else
        return Grade::NONE;
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
        mCharacter->addEngrave(engraveName, engraveValue);
    else
        mCharacter->addPenalty(engraveName, engraveValue);
}

Grade Profile::getGradeByColor(QString color)
{
    if (color.compare("#8DF901", Qt::CaseInsensitive) == 0)
        return Grade::UNCOMMON;
    else if (color.compare("#00B0FA", Qt::CaseInsensitive) == 0)
        return Grade::RARE;
    else if (color.compare("#CE43FC", Qt::CaseInsensitive) == 0)
        return Grade::EPIC;
    else if (color.compare("#F99200", Qt::CaseInsensitive) == 0)
        return Grade::LEGEND;
    else if (color.compare("#FA5D00", Qt::CaseInsensitive) == 0)
        return Grade::RELIC;
    else if (color.compare("#E3C7A1", Qt::CaseInsensitive) == 0)
        return Grade::ANCIENT;
    else if (color.compare("#3CF2E6", Qt::CaseInsensitive) == 0)
        return Grade::ESTHER;
    else
        return Grade::NONE;
}

void Profile::clearAll()
{
    // 이전 캐릭터 정보 초기화
    if (mCharacter != nullptr)
    {
        delete mCharacter;
        mCharacter = nullptr;
    }
    if (mCharacterList != nullptr)
    {
        delete mCharacterList;
        mCharacterList = nullptr;
    }

    for (EquipWidget* equipWidget : mEquipWidgets)
    {
        ui->vLayoutEquip->removeWidget(equipWidget);
        delete equipWidget;
    }
    mEquipWidgets.clear();

    for (AccWidget* accWidget : mAccWidgets)
    {
        ui->vLayoutAccessory->removeWidget(accWidget);
        delete accWidget;
    }
    mAccWidgets.clear();

    if (mStoneWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(mStoneWidget);
        delete mStoneWidget;
        mStoneWidget = nullptr;
    }

    if (mBraceletWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(mBraceletWidget);
        delete mBraceletWidget;
        mBraceletWidget = nullptr;
    }

    if (mEngraveWidget != nullptr)
    {
        ui->vLayoutOther->removeWidget(mEngraveWidget);
        delete mEngraveWidget;
        mEngraveWidget = nullptr;
    }

    for (GemWidget* gemWidget : mGemWidgets)
    {
        ui->vLayoutGem->removeWidget(gemWidget);
        delete gemWidget;
    }
    mGemWidgets.clear();

    for (CardLabel* cardLabel : mCardLabels)
    {
        ui->vLayoutCard->removeWidget(cardLabel);
        delete cardLabel;
    }
    mCardLabels.clear();

    for (SkillWidget* skillWidget : mSkillWidgets)
    {
        ui->vLayoutSkill->removeWidget(skillWidget);
        delete skillWidget;
    }
    mSkillWidgets.clear();
}

void Profile::slotProfileRequest()
{
    // 입력받은 캐릭터명으로 전투정보실에 request를 보냄
    QString name = ui->leName->text();
    QString url = PATH_PROFILE + "/" + name;
    QNetworkRequest request((QUrl(url)));
    mNetworkProfile->get(request);
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
        if (mProfile != nullptr)
            delete mProfile;
        mProfile = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());

        // 기존 데이터 초기화
        clearAll();
        mCharacter = new Character();
        mCharacter->setName(ui->leName->text());
        ui->leName->clear();
        mCharacterList = new CharacterList(nullptr, this);

        // 추출 결과 parsing
        parseItem();
        parseEngrave();
        parseCard();
        parseSkill();
        parseTitle(responseData);
    }
}

void Profile::slotShowCharacterList()
{
    this->setDisabled(true);
    mCharacterList->show();
}

void Profile::slotUpdateTitle()
{
    ui->lbClassLevel->setText(QString("%1 %2").arg(mCharacter->getClass(), mCharacter->getLevel()));
    ui->lbName->setText(mCharacter->getName());
    ui->lbServer->setText(mCharacter->getServer());
    ui->lbGuild->setText(mCharacter->getGuild());
    ui->lbItemLevel->setText(QString("%1").arg(mCharacter->getItemLevel()));
}

void Profile::slotUpdateItem()
{
    const int INDEX_EQUIP = static_cast<int>(Part::WEAPON);
    const int INDEX_ACCESSORY = static_cast<int>(Part::NECK);
    const int INDEX_STONE = static_cast<int>(Part::STONE);

    Part part;
    for (int i = INDEX_EQUIP; i < INDEX_ACCESSORY; i++)
    {
        part = static_cast<Part>(i);
        const Equip& equip = static_cast<const Equip&>(mCharacter->getItemByPart(part));
        EquipWidget* equipWidget = new EquipWidget(nullptr, &equip, PATH_CDN);
        ui->vLayoutEquip->addWidget(equipWidget);
        mEquipWidgets.append(equipWidget);
    }

    for (int i = INDEX_ACCESSORY; i < INDEX_STONE; i++)
    {
        part = static_cast<Part>(i);
        const Accessory& acc = static_cast<const Accessory&>(mCharacter->getItemByPart(part));
        AccWidget* accWidget = new AccWidget(nullptr, &acc, PATH_CDN);
        ui->vLayoutAccessory->addWidget(accWidget);
        mAccWidgets.append(accWidget);
    }

    part = Part::STONE;
    const AbilityStone& stone = static_cast<const AbilityStone&>(mCharacter->getItemByPart(part));
    AbilityStoneWidget* stoneWidget = new AbilityStoneWidget(nullptr, &stone, PATH_CDN);
    ui->vLayoutOther->addWidget(stoneWidget);
    mStoneWidget = stoneWidget;

    part = Part::BRACELET;
    const Bracelet& bracelet = static_cast<const Bracelet&>(mCharacter->getItemByPart(part));
    BraceletWidget* braceletWidget = new BraceletWidget(nullptr, &bracelet, PATH_CDN);
    ui->vLayoutOther->addWidget(braceletWidget);
    mBraceletWidget = braceletWidget;
}

void Profile::slotUpdateGem()
{
    const QList<Gem>& gems = mCharacter->getGems();

    for (const Gem& gem : gems)
    {
        GemWidget* gemWidget = new GemWidget(nullptr, &gem, PATH_CDN);
        ui->vLayoutGem->addWidget(gemWidget);
        mGemWidgets.append(gemWidget);
    }
}

void Profile::slotUpdateEngrave()
{
    EngraveWidget* engraveWidget = new EngraveWidget(nullptr, &mCharacter->getEngrave());
    ui->vLayoutOther->addWidget(engraveWidget);
    mEngraveWidget = engraveWidget;
}

void Profile::slotUpdateCard()
{
    const CardSet& cardSet = mCharacter->getCardSet();

    for (int i = 0; i < cardSet.count(); i++)
    {
        CardLabel* cardLabel = new CardLabel(nullptr, cardSet.getTitle(i), cardSet.getDesc(i));
        ui->vLayoutCard->addWidget(cardLabel);
        mCardLabels.append(cardLabel);
    }
}

void Profile::slotUpdateSkill()
{
    const QList<Skill>& skills = mCharacter->getSkills();

    for (const Skill& skill : skills)
    {
        SkillWidget* skillWidget = new SkillWidget(nullptr, &skill, PATH_CDN);
        ui->vLayoutSkill->addWidget(skillWidget);
        mSkillWidgets.append(skillWidget);
    }
}
