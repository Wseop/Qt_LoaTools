#include "profile.h"
#include "ui_profile.h"
#include "character_list.h"
#include "card_set.h"

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
    mNetworkIconEquip(new QNetworkAccessManager()),
    mNetworkIconGem(new QNetworkAccessManager()),
    mNetworkIconSkill(new QNetworkAccessManager()),
    mHtmlTag("<[^>]*>")
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");
    this->showMaximized();

    initMap();
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

void Profile::initMap()
{
    mPartIcon[Part::WEAPON] = ui->lbIconWeapon;
    mPartIcon[Part::HEAD] = ui->lbIconHead;
    mPartIcon[Part::TOP] = ui->lbIconTop;
    mPartIcon[Part::BOTTOM] = ui->lbIconBottom;
    mPartIcon[Part::HAND] = ui->lbIconHand;
    mPartIcon[Part::SHOULDER] = ui->lbIconShoulder;
    mPartIcon[Part::NECK] = ui->lbIconNeck;
    mPartIcon[Part::EAR1] = ui->lbIconEar1;
    mPartIcon[Part::EAR2] = ui->lbIconEar2;
    mPartIcon[Part::RING1] = ui->lbIconRing1;
    mPartIcon[Part::RING2] = ui->lbIconRing2;
    mPartIcon[Part::STONE] = ui->lbIconStone;
    mPartIcon[Part::BRACELET] = ui->lbIconBracelet;

    mPartQual[Part::WEAPON] = ui->barQualWeapon;
    mPartQual[Part::HEAD] = ui->barQualHead;
    mPartQual[Part::TOP] = ui->barQualTop;
    mPartQual[Part::BOTTOM] = ui->barQualBottom;
    mPartQual[Part::HAND] = ui->barQualHand;
    mPartQual[Part::SHOULDER] = ui->barQualShoulder;
    mPartQual[Part::NECK] = ui->barQualNeck;
    mPartQual[Part::EAR1] = ui->barQualEar1;
    mPartQual[Part::EAR2] = ui->barQualEar2;
    mPartQual[Part::RING1] = ui->barQualRing1;
    mPartQual[Part::RING2] = ui->barQualRing2;

    mPartName[Part::WEAPON] = ui->lbNameWeapon;
    mPartName[Part::HEAD] = ui->lbNameHead;
    mPartName[Part::TOP] = ui->lbNameTop;
    mPartName[Part::BOTTOM] = ui->lbNameBottom;
    mPartName[Part::HAND] = ui->lbNameHand;
    mPartName[Part::SHOULDER] = ui->lbNameShoulder;
    mPartName[Part::NECK] = ui->lbNameNeck;
    mPartName[Part::EAR1] = ui->lbNameEar1;
    mPartName[Part::EAR2] = ui->lbNameEar2;
    mPartName[Part::RING1] = ui->lbNameRing1;
    mPartName[Part::RING2] = ui->lbNameRing2;
    mPartName[Part::STONE] = ui->lbNameStone;
    mPartName[Part::BRACELET] = ui->lbNameBracelet;

    mPartLevel[Part::WEAPON] = ui->lbLevelWeapon;
    mPartLevel[Part::HEAD] = ui->lbLevelHead;
    mPartLevel[Part::TOP] = ui->lbLevelTop;
    mPartLevel[Part::BOTTOM] = ui->lbLevelBottom;
    mPartLevel[Part::HAND] = ui->lbLevelHand;
    mPartLevel[Part::SHOULDER] = ui->lbLevelShoulder;

    mPartSet[Part::WEAPON] = ui->lbSetWeapon;
    mPartSet[Part::HEAD] = ui->lbSetHead;
    mPartSet[Part::TOP] = ui->lbSetTop;
    mPartSet[Part::BOTTOM] = ui->lbSetBottom;
    mPartSet[Part::HAND] = ui->lbSetHand;
    mPartSet[Part::SHOULDER] = ui->lbSetShoulder;

    mPartAttr[Part::NECK] = ui->lbAttrNeck;
    mPartAttr[Part::EAR1] = ui->lbAttrEar1;
    mPartAttr[Part::EAR2] = ui->lbAttrEar2;
    mPartAttr[Part::RING1] = ui->lbAttrRing1;
    mPartAttr[Part::RING2] = ui->lbAttrRing2;
    mPartAttr[Part::BRACELET] = ui->lbEffectBracelet;

    mPartEngrave[Part::NECK] = ui->lbEngraveNeck;
    mPartEngrave[Part::EAR1] = ui->lbEngraveEar1;
    mPartEngrave[Part::EAR2] = ui->lbEngraveEar2;
    mPartEngrave[Part::RING1] = ui->lbEngraveRing1;
    mPartEngrave[Part::RING2] = ui->lbEngraveRing2;
    mPartEngrave[Part::STONE] = ui->lbEngraveStone;

    mGemIcons.append(ui->lbIconGem0);    mGemIcons.append(ui->lbIconGem1);
    mGemIcons.append(ui->lbIconGem2);    mGemIcons.append(ui->lbIconGem3);
    mGemIcons.append(ui->lbIconGem4);    mGemIcons.append(ui->lbIconGem5);
    mGemIcons.append(ui->lbIconGem6);    mGemIcons.append(ui->lbIconGem7);
    mGemIcons.append(ui->lbIconGem8);    mGemIcons.append(ui->lbIconGem9);
    mGemIcons.append(ui->lbIconGem10);

    mGemLevels.append(ui->lbLevelGem0);    mGemLevels.append(ui->lbLevelGem1);
    mGemLevels.append(ui->lbLevelGem2);    mGemLevels.append(ui->lbLevelGem3);
    mGemLevels.append(ui->lbLevelGem4);    mGemLevels.append(ui->lbLevelGem5);
    mGemLevels.append(ui->lbLevelGem6);    mGemLevels.append(ui->lbLevelGem7);
    mGemLevels.append(ui->lbLevelGem8);    mGemLevels.append(ui->lbLevelGem9);
    mGemLevels.append(ui->lbLevelGem10);

    mGemNames.append(ui->lbNameGem0);    mGemNames.append(ui->lbNameGem1);
    mGemNames.append(ui->lbNameGem2);    mGemNames.append(ui->lbNameGem3);
    mGemNames.append(ui->lbNameGem4);    mGemNames.append(ui->lbNameGem5);
    mGemNames.append(ui->lbNameGem6);    mGemNames.append(ui->lbNameGem7);
    mGemNames.append(ui->lbNameGem8);    mGemNames.append(ui->lbNameGem9);
    mGemNames.append(ui->lbNameGem10);

    mGemAttrs.append(ui->lbAttrGem0);    mGemAttrs.append(ui->lbAttrGem1);
    mGemAttrs.append(ui->lbAttrGem2);    mGemAttrs.append(ui->lbAttrGem3);
    mGemAttrs.append(ui->lbAttrGem4);    mGemAttrs.append(ui->lbAttrGem5);
    mGemAttrs.append(ui->lbAttrGem6);    mGemAttrs.append(ui->lbAttrGem7);
    mGemAttrs.append(ui->lbAttrGem8);    mGemAttrs.append(ui->lbAttrGem9);
    mGemAttrs.append(ui->lbAttrGem10);
}

void Profile::initUI()
{
    ui->tabProfile->hide();
    ui->groupTitle->hide();

    ui->groupBoxSearch->setFixedWidth(500);

    ui->barQualWeapon->setFixedSize(50, 15);
    ui->barQualHead->setFixedSize(50, 15);
    ui->barQualShoulder->setFixedSize(50, 15);
    ui->barQualTop->setFixedSize(50, 15);
    ui->barQualBottom->setFixedSize(50, 15);
    ui->barQualHand->setFixedSize(50, 15);
    ui->barQualNeck->setFixedSize(50, 15);
    ui->barQualEar1->setFixedSize(50, 15);
    ui->barQualEar2->setFixedSize(50, 15);
    ui->barQualRing1->setFixedSize(50, 15);
    ui->barQualRing2->setFixedSize(50, 15);

    ui->groupBracelet->setMaximumSize(500, 300);
    ui->groupEngrave->setMaximumWidth(500);

    ui->lbLevelGem0->setFixedWidth(50);
    ui->lbLevelGem1->setFixedWidth(50);
    ui->lbLevelGem2->setFixedWidth(50);
    ui->lbLevelGem3->setFixedWidth(50);
    ui->lbLevelGem4->setFixedWidth(50);
    ui->lbLevelGem5->setFixedWidth(50);
    ui->lbLevelGem6->setFixedWidth(50);
    ui->lbLevelGem7->setFixedWidth(50);
    ui->lbLevelGem8->setFixedWidth(50);
    ui->lbLevelGem9->setFixedWidth(50);
    ui->lbLevelGem10->setFixedWidth(50);

    ui->groupCard->setMaximumWidth(350);

    ui->tabEquip->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabSkill->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
}

void Profile::initConnect()
{
    connect(ui->pbSearch, SIGNAL(pressed()), this, SLOT(slotProfileRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(slotProfileRequest()));
    connect(mNetworkProfile, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotExtractProfile(QNetworkReply*)));
    connect(mNetworkIconEquip, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIconEquip(QNetworkReply*)));
    connect(mNetworkIconGem, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIconGem(QNetworkReply*)));
    connect(mNetworkIconSkill, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIconSkill(QNetworkReply*)));
    connect(ui->pbCharacterList, SIGNAL(pressed()), this, SLOT(slotShowCharacterList()));
}

// 중첩구조인 json 객체로부터 최종 value를 추출
QVariant Profile::getValueFromJson(const QJsonObject& src, QStringList keys)
{
    QJsonObject obj = src;

    for (int i = 0; i < keys.size() - 1; i++)
    {
        obj = obj.find(keys[i])->toObject();
    }
    return obj.find(keys.last())->toVariant();
}

void Profile::parseTitle(QString& profile)
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

    updateTitle();
}

void Profile::parseCharacterList(QString &profile)
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

void Profile::parseGuildName(QString &profile)
{
    qsizetype indexStart = profile.indexOf("game-info__guild");
    indexStart = profile.indexOf("<span>", indexStart) + 1;
    indexStart = profile.indexOf("<span>", indexStart) + 6;
    if (profile[indexStart] == '<')
        indexStart = profile.indexOf(">", indexStart) + 1;
    qsizetype indexEnd = profile.indexOf("</span>", indexStart);

    mCharacter->setGuild(profile.sliced(indexStart, indexEnd - indexStart));
}

// 장비 정보 추출 (무기, 방어구, 악세, 어빌리티 스톤, 팔찌)
void Profile::parseEquip()
{
    const QJsonObject& equip = mProfile->find("Equip")->toObject();
    const QStringList& tempKeys = equip.keys();
    QStringList keys;

    // 장비류 key값 parsing
    for (const QString& key : tempKeys)
    {
        if (key.contains("Gem"))
            continue;
        keys << key;
    }

    QStringList nameKeys;
    nameKeys << "Element_000" << "value";
    QStringList iconKeys;
    iconKeys << "Element_001" << "value" << "slotData" << "iconPath";
    QStringList qualKeys;
    qualKeys << "Element_001" << "value" << "qualityValue";
    QStringList engraveKeys_0;
    engraveKeys_0 << "Element_006" << "value" << "Element_000" << "contentStr" << "Element_000" << "contentStr";
    QStringList engraveKeys_1;
    engraveKeys_1 << "Element_006" << "value" << "Element_000" << "contentStr" << "Element_001" << "contentStr";
    QStringList engraveKeys_2;
    engraveKeys_2 << "Element_006" << "value" << "Element_000" << "contentStr" << "Element_002" << "contentStr";
    QStringList levelKeys;
    levelKeys << "Element_001" << "value" << "leftStr2";
    QStringList setKeys;
    setKeys << "Element_008" << "value" << "Element_001";
    QStringList attrKeys;
    attrKeys << "Element_005" << "value" << "Element_001";

    double itemLevel = 0;
    for (const QString& key : keys)
    {
        if (key.endsWith("0" + QString::number(static_cast<int>(Part::WEAPON))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HEAD))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::TOP))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::BOTTOM))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HAND))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::SHOULDER))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QString name = getValueFromJson(obj, nameKeys).toString();
            QString level = getValueFromJson(obj, levelKeys).toString();
            int quality = getValueFromJson(obj, qualKeys).toInt();
            QString set = getValueFromJson(obj, setKeys).toString();
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            name = name.remove(mHtmlTag);
            level = level.remove(mHtmlTag);
            set = set.remove(mHtmlTag);

            Part part = static_cast<Part>(key.last(3).toInt());
            Equip equip(part);
            equip.setName(name);
            equip.setGrade(getItemGrade(obj));
            equip.setIconPath(iconPath);
            equip.setLevelTier(level);
            equip.setQuality(quality);
            equip.setSetLevel(set);
            mCharacter->setItemByPart(static_cast<const Item&>(equip), part);

            mPathParts[iconPath].append(part);

            // 아이템 level parsing
            int startIndex = level.indexOf("레벨") + 3;
            int endIndex = level.indexOf("(", startIndex) - 1;
            itemLevel += level.sliced(startIndex, endIndex - startIndex).toDouble();
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::NECK))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR2))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING2))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QString name = getValueFromJson(obj, nameKeys).toString();
            int quality = getValueFromJson(obj, qualKeys).toInt();
            QString attr = getValueFromJson(obj, attrKeys).toString();
            QString engrave =
                    getValueFromJson(obj, engraveKeys_0).toString() +
                    getValueFromJson(obj, engraveKeys_1).toString() +
                    getValueFromJson(obj, engraveKeys_2).toString();
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            name = name.remove(mHtmlTag);
            engrave = engrave.replace("#FFFFAC", "#B9B919");

            Part part = static_cast<Part>(key.last(3).toInt());
            Accessory acc(part);
            acc.setName(name);
            acc.setGrade(getItemGrade(obj));
            acc.setIconPath(iconPath);
            acc.setQuality(quality);
            acc.setAttr(attr);
            acc.setEngrave(engrave);
            mCharacter->setItemByPart(static_cast<const Item&>(acc), part);

            mPathParts[iconPath].append(part);
            extractEngraveValue(engrave.remove(mHtmlTag));
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::STONE))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QString name = getValueFromJson(obj, nameKeys).toString();
            QString engrave;
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            name = name.remove(mHtmlTag);
            // 각인 값의 key가 유동적
            // 어느 key에 각인이 있는지 탐색후 값 추출
            QStringList engraveKeys;
            engraveKeys << "Element_005" << "value" << "Element_000";
            if (getValueFromJson(obj, engraveKeys).toJsonObject().contains("contentStr"))
            {
                engraveKeys_0[0] = "Element_005";
                engraveKeys_1[0] = "Element_005";
                engraveKeys_2[0] = "Element_005";
            }
            engrave =
                    getValueFromJson(obj, engraveKeys_0).toString() +
                    getValueFromJson(obj, engraveKeys_1).toString() +
                    getValueFromJson(obj, engraveKeys_2).toString();
            engrave = engrave.replace("#FFFFAC", "#B9B919", Qt::CaseInsensitive);

            Part part = static_cast<Part>(key.last(3).toInt());
            AbilityStone stone;
            stone.setName(name);
            stone.setGrade(getItemGrade(obj));
            stone.setIconPath(iconPath);
            stone.setEngrave(engrave);
            mCharacter->setItemByPart(static_cast<const Item&>(stone), part);

            mPathParts[iconPath].append(part);
            extractEngraveValue(engrave.remove(mHtmlTag));
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::BRACELET))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QStringList braceletEffectKeys;
            braceletEffectKeys << "Element_004" << "value" << "Element_001";

            QString name = getValueFromJson(obj, nameKeys).toString();
            QString effect = getValueFromJson(obj, braceletEffectKeys).toString();
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            name = name.remove(mHtmlTag);
            effect = effect.remove(QRegularExpression("<img[^>]*>"));
            effect = effect.replace("</img>", "-");
            effect = effect.replace("#F9F7D0", "#B9B919", Qt::CaseInsensitive);
            effect = effect.replace("#99FF99", "#0ADC64", Qt::CaseInsensitive);

            Part part = static_cast<Part>(key.last(3).toInt());
            Bracelet bracelet;
            bracelet.setName(name);
            bracelet.setGrade(getItemGrade(obj));
            bracelet.setIconPath(iconPath);
            bracelet.setEffect(effect);
            mCharacter->setItemByPart(static_cast<const Item&>(bracelet), part);

            mPathParts[iconPath].append(part);
        }
    }
    mCharacter->setItemLevel(itemLevel / 6);

    updateEquip();
}

// 보석 정보 추출
void Profile::parseGem()
{
    const QJsonObject& equip = mProfile->find("Equip")->toObject();
    const QStringList& tempKeys = equip.keys();
    QStringList keys;

    // 장비류 key값 parsing
    for (const QString& key : tempKeys)
    {
        if (key.contains("Gem"))
            keys << key;
    }

    QStringList nameKeys;
    nameKeys << "Element_000" << "value";
    QStringList iconKeys;
    iconKeys << "Element_001" << "value" << "slotData" << "iconPath";
    QStringList levelKeys;
    levelKeys << "Element_001" << "value" << "slotData" << "rtString";
    QStringList attrKeys;
    attrKeys << "Element_004" << "value" << "Element_001";

    for (int i = 0; i < keys.size(); i++)
    {
        const QJsonObject& obj = equip.find(keys[i])->toObject();

        QString name = getValueFromJson(obj, nameKeys).toString();
        QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();
        QString level = getValueFromJson(obj, levelKeys).toString();
        QString attr = getValueFromJson(obj, attrKeys).toString();

        name = name.remove(mHtmlTag);
        attr = attr.replace("#FFD200", "#B9B919", Qt::CaseInsensitive);

        Gem gem;
        gem.setName(name);
        gem.setGrade(getItemGrade(obj));
        gem.setIconPath(iconPath);
        gem.setLevel(level);
        gem.setAttr(attr);
        mCharacter->addGem(gem);

        mGemPathIndex[iconPath].append(i);
    }

    updateGem();
}

// 각인 정보 추출
void Profile::parseEngrave()
{
    const QJsonObject& engrave = mProfile->find("Engrave")->toObject();
    QStringList keys = engrave.keys();

    QStringList nameKeys;
    nameKeys << "Element_000" << "value";
    QStringList valueKeys;
    valueKeys << "Element_001" << "value" << "leftText";

    for (const QString& key : keys)
    {
        const QJsonObject& obj = engrave.find(key)->toObject();

        QString name = getValueFromJson(obj, nameKeys).toString();
        QString valueStr = getValueFromJson(obj, valueKeys).toString();

        valueStr = valueStr.remove(mHtmlTag);
        int value = valueStr.remove("각인 활성 포인트 +").toInt();

        mCharacter->addEngrave(name, value);
    }

    updateEngrave();
}

void Profile::parseSkill()
{
    const QJsonObject& skill = mProfile->find("Skill")->toObject();
    QStringList keys = skill.keys();

    QStringList iconKeys;
    iconKeys << "value" << "slotData" << "iconPath";

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
                QString iconPath = "/" + getValueFromJson(data, iconKeys).toString();
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

    updateSkill();
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
    updateCard();
}

void Profile::extractEngraveValue(QString engrave)
{
    qsizetype start = 0;
    qsizetype end = 0;
    qsizetype from = 0;
    QString name;
    int value;

    // 각인 이름, 값 추출 - 1
    start = engrave.indexOf("[") + 1;
    end = engrave.indexOf("]");
    name = engrave.sliced(start, end - start);

    start = engrave.indexOf("+") + 1;
    // 예외처리1) 아이템 각인이 1줄밖에 없는 경우
    if (start + 1 == engrave.size())
    {
        value = engrave[start].digitValue();
        mCharacter->addEngrave(name, value);
        return;
    }

    if (engrave[start + 1] == '[')
        value = engrave[start].digitValue();
    else
        value = engrave.sliced(start, 2).toInt();
    mCharacter->addEngrave(name, value);

    // 각인 이름, 값 추출 - 2
    from = start + 1;
    start = engrave.indexOf("[", from) + 1;
    end = engrave.indexOf("]", from);
    name = engrave.sliced(start, end - start);

    start = engrave.indexOf("+", from) + 1;
    if (engrave[start + 1] == '[')
        value = engrave[start].digitValue();
    else
        value = engrave.sliced(start, 2).toInt();
    mCharacter->addEngrave(name, value);

    // 각인 이름, 값 추출 - 3 (Penalty)
    from = start + 1;
    start = engrave.indexOf("[", from) + 1;
    end = engrave.indexOf("]", from);
    name = engrave.sliced(start, end - start);

    start = engrave.indexOf("+", from) + 1;
    if (start == engrave.size() - 1)
        value = engrave[start].digitValue();
    else
        value = engrave.sliced(start, 2).toInt();
    mCharacter->addPenalty(name, value);
}

void Profile::updateTitle()
{
    ui->lbClassLevel->setText(QString("%1 %2").arg(mCharacter->getClass(), mCharacter->getLevel()));
    ui->lbName->setText(mCharacter->getName());
    ui->lbServer->setText(mCharacter->getServer());
    ui->lbServer->setStyleSheet("QLabel { color: #B178FF }");
    ui->lbGuild->setText(mCharacter->getGuild());
    ui->lbGuild->setStyleSheet("QLabel { color: #00B700 }");
    ui->lbItemLevel->setText(QString("%1").arg(mCharacter->getItemLevel()));
    ui->lbItemLevel->setStyleSheet("QLabel { color: #FF009B }");
    ui->vLayoutTitle->setAlignment(Qt::AlignTop);
}

void Profile::updateEquip()
{
    const int START_EQUIP = 0;
    const int START_ACCESSORY = 6;
    const int START_STONE = 11;
    const int START_BRACELET = 26;

    Part part;
    // Equip
    for (int i = START_EQUIP; i < START_ACCESSORY; i++)
    {
        part = static_cast<Part>(i);
        const Equip& equip = static_cast<const Equip&>(mCharacter->getItemByPart(part));

        requestIcon(mNetworkIconEquip, equip.getIconPath());
        mPartName[part]->setText(equip.getName());
        setNameColor(mPartName[part], equip.getGrade());
        mPartQual[part]->setValue(equip.getQuality());
        setQualityColor(part, equip.getQuality());
        mPartLevel[part]->setText(equip.getLevelTier());
        mPartSet[part]->setText(equip.getSetLevel());
    }
    // Accessory
    for (int i = START_ACCESSORY; i < START_STONE; i++)
    {
        part = static_cast<Part>(i);
        const Accessory& acc = static_cast<const Accessory&>(mCharacter->getItemByPart(part));

        requestIcon(mNetworkIconEquip, acc.getIconPath());
        mPartName[part]->setText(acc.getName());
        setNameColor(mPartName[part], acc.getGrade());
        mPartQual[part]->setValue(acc.getQuality());
        setQualityColor(part, acc.getQuality());
        mPartAttr[part]->setText(acc.getAttr());
        mPartEngrave[part]->setText(acc.getEngrave());
    }
    // Ability Stone
    part = static_cast<Part>(START_STONE);
    const AbilityStone& stone = static_cast<const AbilityStone&>(mCharacter->getItemByPart(part));

    requestIcon(mNetworkIconEquip, stone.getIconPath());
    mPartName[part]->setText(stone.getName());
    setNameColor(mPartName[part], stone.getGrade());
    mPartEngrave[part]->setText(stone.getEngrave());

    // Bracelet
    part = static_cast<Part>(START_BRACELET);
    const Bracelet& bracelet = static_cast<const Bracelet&>(mCharacter->getItemByPart(part));

    requestIcon(mNetworkIconEquip, bracelet.getIconPath());
    mPartName[part]->setText(bracelet.getName());
    setNameColor(mPartName[part], bracelet.getGrade());
    mPartAttr[part]->setText(bracelet.getEffect());
}

void Profile::updateGem()
{
    const QList<Gem>& gems = mCharacter->getGems();

    for (int i = 0; i < gems.size(); i++)
    {
        const Gem& gem = gems[i];

        requestIcon(mNetworkIconGem, gem.getIconPath());
        mGemNames[i]->setText(gem.getName());
        setNameColor(mGemNames[i], gem.getGrade());
        mGemLevels[i]->setText(gem.GetLevel());
        mGemAttrs[i]->setText(gem.getAttr());
    }
}

void Profile::updateEngrave()
{
    const Engrave& engrave = mCharacter->getEngrave();
    QStringList engraveList = engrave.getActiveEngraveList();
    QStringList penaltyList = engrave.getActivePenaltyList();

    // level 3 -> 2 -> 1 순으로 ui 추가
    for (int i = 3; i >= 1; i--)
    {
        for (const QString& engraveName : engraveList)
        {
            int value = engrave.getEngraveValue(engraveName);
            int level = value / 5;
            if (level != i)
                continue;

            QHBoxLayout* layout = new QHBoxLayout();

            QLabel* lbIcon = new QLabel();
            QLabel* lbName = new QLabel();
            QLabel* lbLevel = new QLabel();
            layout->addWidget(lbIcon);
            layout->addWidget(lbName);
            layout->addWidget(lbLevel);
            mEngraveLabels.append(lbIcon);
            mEngraveLabels.append(lbName);
            mEngraveLabels.append(lbLevel);

            lbName->setText(engraveName);
            lbLevel->setText(QString("Lv. %1").arg(level));

            QString iconPath = engrave.getEngravePath(engraveName);
            QPixmap pixmap(iconPath);
            lbIcon->setPixmap(pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            ui->vLayoutEngrave->addLayout(layout);
            mEngraveLayouts.append(layout);

            lbIcon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbName->setFont(QFont("나눔스퀘어 네오 Bold", 12));
            lbName->setMaximumHeight(50);
            lbLevel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbLevel->setFont(QFont("나눔스퀘어 네오 Bold", 12));
            lbLevel->setMaximumHeight(50);
        }

        for (const QString& penalty : penaltyList)
        {
            int value = engrave.getPenaltyValue(penalty);
            int level = value / 5;
            if (level != i)
                continue;

            QHBoxLayout* layout = new QHBoxLayout();

            QLabel* lbIcon = new QLabel();
            QLabel* lbName = new QLabel();
            QLabel* lbLevel = new QLabel();
            layout->addWidget(lbIcon);
            layout->addWidget(lbName);
            layout->addWidget(lbLevel);
            mEngraveLabels.append(lbIcon);
            mEngraveLabels.append(lbName);
            mEngraveLabels.append(lbLevel);

            lbName->setText(penalty);
            lbLevel->setText(QString("Lv. %1").arg(level));

            QString iconPath = engrave.getPenaltyPath(penalty);
            QPixmap pixmap(iconPath);
            lbIcon->setPixmap(pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            ui->vLayoutPenalty->addLayout(layout);
            mPenaltyLayouts.append(layout);

            lbName->setStyleSheet("QLabel { color: red }");
            lbLevel->setStyleSheet("QLabel { color: red }");
            lbIcon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbName->setFont(QFont("나눔스퀘어 네오 Bold", 12));
            lbName->setMaximumHeight(50);
            lbLevel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            lbLevel->setFont(QFont("나눔스퀘어 네오 Bold", 12));
            lbLevel->setMaximumHeight(50);
        }
    }
}

void Profile::updateSkill()
{
    const QList<Skill>& skills = mCharacter->getSkills();

    for (const Skill& skill : skills)
    {
        QGroupBox* groupSkill = new QGroupBox();
        mSkillGroupBoxes.append(groupSkill);
        ui->vLayoutSkill->addWidget(groupSkill);

        QHBoxLayout* hLayoutSkill = new QHBoxLayout();
        mSkillLayouts.append(hLayoutSkill);
        groupSkill->setLayout(hLayoutSkill);
        hLayoutSkill->setAlignment(Qt::AlignLeft);

        // 스킬 아이콘
        QLabel* lbSkillIcon = new QLabel();
        mSkillLabels.append(lbSkillIcon);
        hLayoutSkill->addWidget(lbSkillIcon);
        mSkillIconLabel[skill.getIconPath()] = lbSkillIcon;
        requestIcon(mNetworkIconSkill, skill.getIconPath());

        // 스킬명, 레벨
        QLabel* lbSkillNameLevel = new QLabel(QString("%1 Lv.%2").arg(skill.getName()).arg(skill.getLevel()));
        mSkillLabels.append(lbSkillNameLevel);
        hLayoutSkill->addWidget(lbSkillNameLevel);
        lbSkillNameLevel->setFont(QFont("나눔스퀘어 네오 Bold", 10));
        lbSkillNameLevel->setFixedWidth(200);

        // 트라이포드
        QGroupBox* groupTripod = new QGroupBox("트라이포드");
        mSkillGroupBoxes.append(groupTripod);
        hLayoutSkill->addWidget(groupTripod);
        groupTripod->setFont(QFont("나눔스퀘어 네오 regular", 10));
        groupTripod->setFixedWidth(250);

        QVBoxLayout* vLayoutTripod = new QVBoxLayout();
        mSkillLayouts.append(vLayoutTripod);
        groupTripod->setLayout(vLayoutTripod);

        QList<Tripod> tripods = skill.getTripods();
        for (int i = 0; i < tripods.size(); i++)
        {
            const Tripod& tripod = tripods[i];
            QHBoxLayout* hLayoutTripod = new QHBoxLayout();
            mSkillLayouts.append(hLayoutTripod);
            vLayoutTripod->addLayout(hLayoutTripod);

            QLabel* lbTripodNameLevel = new QLabel(QString("%1: %2 Lv.%3").arg(i + 1).arg(tripod.name).arg(tripod.level));
            mSkillLabels.append(lbTripodNameLevel);
            hLayoutTripod->addWidget(lbTripodNameLevel);
            lbTripodNameLevel->setFont(QFont("나눔스퀘어 네오 Bold", 10));
            lbTripodNameLevel->setStyleSheet(QString("QLabel { color: %1 }").arg(tripod.color));
        }

        // 룬
        QGroupBox* groupRune = new QGroupBox("룬");
        mSkillGroupBoxes.append(groupRune);
        hLayoutSkill->addWidget(groupRune);
        groupRune->setFont(QFont("나눔스퀘어 네오 regular", 10));
        groupRune->setFixedWidth(200);

        QHBoxLayout* hLayoutRune = new QHBoxLayout();
        mSkillLayouts.append(hLayoutRune);
        groupRune->setLayout(hLayoutRune);

        QLabel* lbRuneIcon = new QLabel();
        mSkillLabels.append(lbRuneIcon);
        hLayoutRune->addWidget(lbRuneIcon);

        const Rune* rune = skill.getRune();
        if (rune != nullptr)
        {
            QPixmap pixmap(rune->getIconPath());
            lbRuneIcon->setPixmap(pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

            QLabel* lbRuneName = new QLabel(rune->getName());
            mSkillLabels.append(lbRuneName);
            hLayoutRune->addWidget(lbRuneName);
            lbRuneName->setFont(QFont("나눔스퀘어 네오 Bold", 10));
            lbRuneName->setStyleSheet(QString("QLabel { color: %1 }").arg(getColorByGrade(rune->getGrade())));
        }
    }
}

void Profile::updateCard()
{
    const CardSet& cardSet = mCharacter->getCardSet();
    qsizetype effectCount = cardSet.count();

    for (int i = 0; i < effectCount; i++)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        mCardLayoutList.append(layout);

        QLabel* lbTitle = new QLabel(cardSet.getTitle(i));
        QLabel* lbDesc = new QLabel(cardSet.getDesc(i));
        lbTitle->setFont(QFont("나눔스퀘어 네오 Bold", 12));
        lbDesc->setFont(QFont("나눔스퀘어 네오 Bold", 12));
        lbTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        lbDesc->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        lbTitle->setStyleSheet("QLabel { color: #00B700 }");
        lbTitle->setMaximumWidth(300);
        lbDesc->setMinimumHeight(50);
        lbDesc->setMaximumWidth(300);
        lbDesc->setWordWrap(true);
        mCardLabelList.append(lbTitle);
        mCardLabelList.append(lbDesc);
        layout->addWidget(lbTitle);
        layout->addWidget(lbDesc);

        ui->vLayoutCard->addLayout(layout);
    }
}

void Profile::requestIcon(QNetworkAccessManager* networkManager, QString iconPath)
{
    QNetworkRequest request((QUrl(PATH_CDN + iconPath)));
    networkManager->get(request);
}

void Profile::setQualityColor(Part part, int quality)
{
    QString color;

    if (quality == 100)
        color = "#FE9600";
    else if (quality >= 90)
        color = "#CE43FC";
    else if (quality >= 70)
        color = "#00B5FF";
    else if (quality >= 30)
        color = "#00B700";
    else if (quality >= 10)
        color = "#FFD200";
    else
        color = "#FF6000";

    QString style = QString("QProgressBar::chunk { background-color: %1 }").arg(color);
    mPartQual[part]->setStyleSheet(style);
}

void Profile::setNameColor(QLabel* label, Grade grade)
{
    QString color;

    if (grade == Grade::UNCOMMON)
        color = "#8DF901";
    else if (grade == Grade::RARE)
        color = "#00B0FA";
    else if (grade == Grade::EPIC)
        color = "#CE43FC";
    else if (grade == Grade::LEGEND)
        color = "#F99200";
    else if (grade == Grade::RELIC)
        color = "#FA5D00";
    else if (grade == Grade::ANCIENT)
        color = "#E3C7A1";
    else if (grade == Grade::ESTHER)
        color = "#3CF2E6";

    QString style = QString("QLabel { color: %1 }").arg(color);
    label->setStyleSheet(style);
    label->setFont(QFont("나눔스퀘어 네오 ExtraBold", 10));
}

Grade Profile::getItemGrade(const QJsonObject& obj)
{
    QStringList keys;
    keys << "Element_001" << "value" << "leftStr0";

    QString itemCategory = getValueFromJson(obj, keys).toString();

    if (itemCategory.contains("고급"))
        return Grade::UNCOMMON;
    else if (itemCategory.contains("희귀"))
        return Grade::RARE;
    else if (itemCategory.contains("영웅"))
        return Grade::EPIC;
    else if (itemCategory.contains("전설"))
        return Grade::LEGEND;
    else if (itemCategory.contains("유물"))
        return Grade::RELIC;
    else if (itemCategory.contains("고대"))
        return Grade::ANCIENT;
    else if (itemCategory.contains("에스더"))
        return Grade::ESTHER;
    else
        return Grade::NONE;
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

QString Profile::getColorByGrade(Grade grade)
{
    switch (grade)
    {
    case Grade::UNCOMMON:
        return "#8DF901";
    case Grade::RARE:
        return "#00B0FA";
    case Grade::EPIC:
        return "#CE43FC";
    case Grade::LEGEND:
        return "#F99200";
    case Grade::RELIC:
        return "#FA5D00";
    case Grade::ANCIENT:
        return "#E3C7A1";
    case Grade::ESTHER:
        return "#3CF2E6";
    case Grade::NONE:
        return "#000000";
    }
}

void Profile::clearAll()
{
    // 이전 캐릭터 정보 모두 초기화
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
    mPathParts.clear();
    mGemPathIndex.clear();

    QList<Part> keys;
    keys = mPartIcon.keys();
    for (Part& key : keys)
        mPartIcon[key]->clear();

    keys = mPartQual.keys();
    for (Part& key : keys)
        mPartQual[key]->reset();

    keys = mPartName.keys();
    for (Part& key : keys)
        mPartName[key]->clear();

    keys = mPartLevel.keys();
    for (Part& key : keys)
        mPartLevel[key]->clear();

    keys = mPartSet.keys();
    for (Part& key : keys)
        mPartSet[key]->clear();

    keys = mPartAttr.keys();
    for (Part& key : keys)
        mPartAttr[key]->clear();

    keys = mPartEngrave.keys();
    for (Part& key : keys)
        mPartEngrave[key]->clear();

    for (QLabel* label : mGemIcons)
        label->clear();

    for (QLabel* label : mGemLevels)
        label->clear();

    for (QLabel* label : mGemNames)
        label->clear();

    for (QLabel* label : mGemAttrs)
        label->clear();

    for (QLabel* label : mEngraveLabels)
        delete label;
    mEngraveLabels.clear();

    for (QHBoxLayout* layout : mEngraveLayouts)
        delete layout;
    mEngraveLayouts.clear();

    for (QHBoxLayout* layout : mPenaltyLayouts)
        delete layout;
    mPenaltyLayouts.clear();

    for (QLabel* label : mCardLabelList)
        delete label;
    mCardLabelList.clear();

    for (QVBoxLayout* layout : mCardLayoutList)
        delete layout;
    mCardLayoutList.clear();

    for (QLabel* label : mSkillLabels)
        delete label;
    mSkillLabels.clear();

    for (auto rIter = mSkillLayouts.rbegin(); rIter != mSkillLayouts.rend(); rIter++)
        delete *rIter;
    mSkillLayouts.clear();

    for (auto rIter = mSkillGroupBoxes.rbegin(); rIter != mSkillGroupBoxes.rend(); rIter++)
        delete *rIter;
    mSkillGroupBoxes.clear();

    mSkillIconLabel.clear();
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

        // Profile 정보 추출
        profileIndex += profileStart.size();
        profileSize = responseData.indexOf(profileEnd) - profileIndex + 1;
        const QString& profile = responseData.sliced(profileIndex, profileSize);
        if (mProfile != nullptr)
            delete mProfile;
        mProfile = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());

        clearAll();
        // 추출 결과 parsing
        mCharacter = new Character();
        mCharacter->setName(ui->leName->text());
        ui->leName->clear();
        mCharacterList = new CharacterList(nullptr, this);

        parseEquip();
        parseGem();
        parseEngrave();
        parseSkill();
        parseCard();
        parseTitle(responseData);
    }
}

void Profile::slotSetIconEquip(QNetworkReply* reply)
{
    QPixmap icon;
    bool load = icon.loadFromData(reply->readAll(), "PNG");
    if (!load)
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    QString path = reply->url().path();
    QList<Part> parts = mPathParts[path];
    for (const Part& part : parts)
    {
        QLabel* iconLabel = mPartIcon[part];

        iconLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        iconLabel->setFixedSize(50, 50);
        iconLabel->setStyleSheet("QLabel { border: 1px solid black }");
    }
}

void Profile::slotSetIconGem(QNetworkReply* reply)
{
    QPixmap icon;
    bool load = icon.loadFromData(reply->readAll(), "PNG");
    if (!load)
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    QString path = reply->url().path();
    QList<int> index = mGemPathIndex[path];
    for (const int& i : index)
    {
        QLabel* iconLabel = mGemIcons[i];

        iconLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        iconLabel->setFixedSize(50, 50);
        iconLabel->setStyleSheet("QLabel { border: 1px solid black }");
    }
}

void Profile::slotSetIconSkill(QNetworkReply *reply)
{
    QPixmap icon;
    bool load = icon.loadFromData(reply->readAll(), "PNG");
    if (!load)
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    QString path = reply->url().path();
    QLabel* label = mSkillIconLabel[path];
    label->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    label->setFixedSize(50, 50);
}

void Profile::slotShowCharacterList()
{
    this->setDisabled(true);
    mCharacterList->show();
}
