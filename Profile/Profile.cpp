#include "profile.h"
#include "ui_profile.h"
#include "character_list.h"

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

    ui->tabEquip->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabSkill->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabCard->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
}

void Profile::initConnect()
{
    connect(ui->pbSearch, SIGNAL(pressed()), this, SLOT(slotProfileRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(slotProfileRequest()));
    connect(mNetworkProfile, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotExtractProfile(QNetworkReply*)));
    connect(mNetworkIconEquip, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIconEquip(QNetworkReply*)));
    connect(mNetworkIconGem, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIconGem(QNetworkReply*)));
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

    // 보유 캐릭터 목록 추출
    parseCharacterList(profile);

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

// 장비 정보 추출 (무기, 방어구, 악세, 어빌리티 스톤, 팔찌)
void Profile::parseEquip()
{
    QJsonObject equip = mProfile->find("Equip")->toObject();
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
            Equip& equip = static_cast<Equip&>(mCharacter->getItemByPart(part));

            equip.setName(name);
            equip.setGrade(getItemGrade(obj));
            equip.setIconPath(iconPath);
            equip.setLevelTier(level);
            equip.setQuality(quality);
            equip.setSetLevel(set);

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
            Accessory& acc = static_cast<Accessory&>(mCharacter->getItemByPart(part));

            acc.setName(name);
            acc.setGrade(getItemGrade(obj));
            acc.setIconPath(iconPath);
            acc.setQuality(quality);
            acc.setAttr(attr);
            acc.setEngrave(engrave);

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
            AbilityStone& stone = static_cast<AbilityStone&>(mCharacter->getItemByPart(part));

            stone.setName(name);
            stone.setGrade(getItemGrade(obj));
            stone.setIconPath(iconPath);
            stone.setEngrave(engrave);

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
            Bracelet& bracelet = static_cast<Bracelet&>(mCharacter->getItemByPart(part));

            bracelet.setName(name);
            bracelet.setGrade(getItemGrade(obj));
            bracelet.setIconPath(iconPath);
            bracelet.setEffect(effect);

            mPathParts[iconPath].append(part);
        }
    }
    mCharacter->setItemLevel(itemLevel / 6);

    updateEquip();
}

// 보석 정보 추출
void Profile::parseGem()
{
    QJsonObject equip = mProfile->find("Equip")->toObject();
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
        mCharacter->getGems().append(gem);

        mGemPathIndex[iconPath].append(i);
    }

    updateGem();
}

// 각인 정보 추출
void Profile::parseEngrave()
{
    QJsonObject engrave = mProfile->find("Engrave")->toObject();
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

        mCharacter->getEngrave().addEngrave(name, value);
    }

    updateEngrave();
}

void Profile::parseSkill()
{}

void Profile::parseCard()
{}

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
    if (engrave[start + 1] == '[')
        value = engrave[start].digitValue();
    else
        value = engrave.sliced(start, 2).toInt();
    mCharacter->getEngrave().addEngrave(name, value);

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
    mCharacter->getEngrave().addEngrave(name, value);

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
    mCharacter->getEngrave().addPenalty(name, value);
}

void Profile::updateTitle()
{
    ui->lbClassLevel->setText(QString("%1 %2").arg(mCharacter->getClass(), mCharacter->getLevel()));
    ui->lbName->setText(mCharacter->getName());
    ui->lbServer->setText(mCharacter->getServer());
    ui->lbServer->setStyleSheet("QLabel { color: #B178FF }");
    ui->lbItemLevel->setText(QString("%1").arg(mCharacter->getItemLevel()));
    ui->lbItemLevel->setStyleSheet("QLabel { color: #FF009B }");
    ui->verticalLayout_59->setAlignment(Qt::AlignTop);
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
        Equip& equip = static_cast<Equip&>(mCharacter->getItemByPart(part));

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
        Accessory& acc = static_cast<Accessory&>(mCharacter->getItemByPart(part));

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
    AbilityStone& stone = static_cast<AbilityStone&>(mCharacter->getItemByPart(part));

    requestIcon(mNetworkIconEquip, stone.getIconPath());
    mPartName[part]->setText(stone.getName());
    setNameColor(mPartName[part], stone.getGrade());
    mPartEngrave[part]->setText(stone.getEngrave());

    // Bracelet
    part = static_cast<Part>(START_BRACELET);
    Bracelet& bracelet = static_cast<Bracelet&>(mCharacter->getItemByPart(part));

    requestIcon(mNetworkIconEquip, bracelet.getIconPath());
    mPartName[part]->setText(bracelet.getName());
    setNameColor(mPartName[part], bracelet.getGrade());
    mPartAttr[part]->setText(bracelet.getEffect());
}

void Profile::updateGem()
{
    QList<Gem>& gems = mCharacter->getGems();

    for (int i = 0; i < gems.size(); i++)
    {
        Gem& gem = gems[i];

        requestIcon(mNetworkIconGem, gem.getIconPath());
        mGemNames[i]->setText(gem.getName());
        setNameColor(mGemNames[i], gem.getGrade());
        mGemLevels[i]->setText(gem.GetLevel());
        mGemAttrs[i]->setText(gem.getAttr());
    }
}

void Profile::updateEngrave()
{
    Engrave& engrave = mCharacter->getEngrave();
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

}

void Profile::updateCard()
{

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
        color = "#AE43FC";
    else if (grade == Grade::LEGEND)
        color = "#F99200";
    else if (grade == Grade::RELIC)
        color = "#FA5000";
    else if (grade == Grade::ANCIENT)
        color = "#D2A76A";
    else if (grade == Grade::ESTHER)
        color = "#3CE6B9";

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
    {
        ui->vLayoutEngrave->removeItem(layout);
        delete layout;
    }
    mEngraveLayouts.clear();

    for (QHBoxLayout* layout : mPenaltyLayouts)
    {
        ui->vLayoutPenalty->removeItem(layout);
        delete layout;
    }
    mPenaltyLayouts.clear();
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
    for (Part& part : parts)
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
    for (int& i : index)
    {
        QLabel* iconLabel = mGemIcons[i];

        iconLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        iconLabel->setFixedSize(50, 50);
        iconLabel->setStyleSheet("QLabel { border: 1px solid black }");
    }
}

void Profile::slotShowCharacterList()
{
    this->setDisabled(true);
    mCharacterList->show();
}
