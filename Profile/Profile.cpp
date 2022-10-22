#include "profile.h"
#include "ui_profile.h"

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

// 장비 정보 추출 (무기, 방어구, 악세, 보석)
void Profile::parseEquip()
{
    // Equip key의 값으로 Equip(장비류)와 보석 정보가 들어있음
    QJsonObject equip = mProfile->find("Equip")->toObject();
    QStringList equipKeys;   // 무기, 방어구, 악세 key값
    QStringList gemKeys;  // 보석 key값

    // key값 추출
    const QStringList& keys = equip.keys();
    for (const QString& key : keys)
    {
        // key값이 GEM을 포함하면 보석류, 아니면 gear류
        if (key.contains("Gem"))
            gemKeys.append(key);
        else
            equipKeys.append(key);
    }

    updateEquip(equip, equipKeys);
    updateGem(equip, gemKeys);
}

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
        // 각인 값에서 수치만 parsing
        int value = valueStr.remove("각인 활성 포인트 +").toInt();

        // 각인 - 값 update
        mEngraveValue[name] += value;
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
    mEngraveValue[name] += value;

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
    mEngraveValue[name] += value;

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
    mEngraveValue[name] += value;
}

void Profile::updateEquip(QJsonObject& equip, QStringList& keys)
{
    // 장비 정보 추출
    QStringList nameKeys;
    nameKeys << "Element_000" << "value";
    QStringList iconKeys;
    iconKeys << "Element_001" << "value" << "slotData" << "iconPath";
    QStringList qualKeys;
    qualKeys << "Element_001" << "value" << "qualityValue";
    QStringList engraveKeys1;
    engraveKeys1 << "Element_005" << "value" << "Element_001";
    QStringList engraveKeys2;
    engraveKeys2 << "Element_006" << "value" << "Element_001";
    QStringList levelKeys;
    levelKeys << "Element_001" << "value" << "leftStr2";
    QStringList setKeys;
    setKeys << "Element_008" << "value" << "Element_001";
    QStringList attrKeys;
    attrKeys << "Element_005" << "value" << "Element_001";

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

            // update ui
            Part part = static_cast<Part>(key.last(3).toInt());

            mPathParts[iconPath].append(part);
            requestIcon(mNetworkIconEquip, iconPath);

            mPartQual[part]->setValue(quality);
            setQualityColor(part, quality);

            mPartName[part]->setText(name);
            setNameColor(mPartName[part], getItemGrade(obj));

            mPartLevel[part]->setText(level);

            mPartSet[part]->setText(set);
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
            QString engrave = getValueFromJson(obj, engraveKeys2).toString();
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            name = name.remove(mHtmlTag);
            engrave = engrave.replace("#FFFFAC", "#B9B919");

            // update ui
            Part part = static_cast<Part>(key.last(3).toInt());

            mPathParts[iconPath].append(part);
            requestIcon(mNetworkIconEquip, iconPath);

            mPartName[part]->setText(name);
            setNameColor(mPartName[part], getItemGrade(obj));

            mPartQual[part]->setValue(quality);
            setQualityColor(part, quality);

            mPartAttr[part]->setText(attr);
            mPartEngrave[part]->setText(engrave);

            extractEngraveValue(engrave.remove(mHtmlTag));
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::STONE))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QString name = getValueFromJson(obj, nameKeys).toString();
            QString engrave;
            QString iconPath = "/" + getValueFromJson(obj, iconKeys).toString();

            // 각인 값의 key가 유동적
            // 어느 key에 각인이 있는지 탐색후 값 추출
            QStringList engraveKeys;
            engraveKeys << "Element_005" << "value" << "Element_000";
            if (getValueFromJson(obj, engraveKeys).toString().contains("각인"))
                engrave = getValueFromJson(obj, engraveKeys1).toString();
            else
                engrave = getValueFromJson(obj, engraveKeys2).toString();

            name = name.remove(mHtmlTag);
            engrave = engrave.replace("#FFFFAC", "#B9B919", Qt::CaseInsensitive);

            // update ui
            Part part = static_cast<Part>(key.last(3).toInt());

            mPathParts[iconPath].append(part);
            requestIcon(mNetworkIconEquip, iconPath);

            mPartName[part]->setText(name);
            setNameColor(mPartName[part], getItemGrade(obj));

            mPartEngrave[part]->setText(engrave);

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

            // update ui
            Part part = static_cast<Part>(key.last(3).toInt());

            mPathParts[iconPath].append(part);
            requestIcon(mNetworkIconEquip, iconPath);

            mPartName[part]->setText(name);
            setNameColor(mPartName[part], getItemGrade(obj));

            mPartAttr[part]->setText(effect);
        }
    }
}

void Profile::updateGem(QJsonObject& equip, QStringList& keys)
{
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

        // update ui
        mGemPathIndex[iconPath].append(i);
        requestIcon(mNetworkIconGem, iconPath);

        mGemLevels[i]->setText(level);

        mGemNames[i]->setText(name);
        setNameColor(mGemNames[i], getItemGrade(obj));

        mGemAttrs[i]->setText(attr);
    }
}

void Profile::updateEngrave()
{
    QStringList engraves = mEngraveValue.keys();

    // level 3 -> 2 -> 1 순으로 ui 추가
    for (int i = 3; i >= 1; i--)
    {
        for (const QString& engrave : engraves)
        {
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

            // 값 세팅
            lbName->setText(engrave);

            int value = mEngraveValue[engrave];
            int level = value / 5;
            lbLevel->setText(QString("Lv. %1").arg(level));

            if (level != i)
                continue;
            if (mEngrave.isValidPenalty(engrave))
            {
                // penalty 각인
                QString iconPath = mEngrave.getPenaltyPath(engrave);
                QPixmap pixmap(iconPath);
                lbIcon->setPixmap(pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

                ui->vLayoutPenalty->addLayout(layout);
                mPenaltyLayouts.append(layout);

                lbName->setStyleSheet("QLabel { color: red }");
                lbLevel->setStyleSheet("QLabel { color: red }");
            }
            else
            {
                // 증가 각인
                QString iconPath = mEngrave.getEngravePath(engrave);
                QPixmap pixmap(iconPath);
                lbIcon->setPixmap(pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

                ui->vLayoutEngrave->addLayout(layout);
                mEngraveLayouts.append(layout);
            }

            // ui 세팅
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
        color = "rgb(0, 183, 0)";
    else if (quality >= 10)
        color = "#FFD200";
    else
        color = "#FF6000";

    QString style = QString("QProgressBar::chunk { background-color: %1 }").arg(color);
    mPartQual[part]->setStyleSheet(style);
}

void Profile::setNameColor(QLabel* label, int grade)
{
    QString color;

    if (grade == 0)
        color = "#F99200";
    else if (grade == 1)
        color = "#FA5000";
    else if (grade == 2)
        color = "rgb(210, 166, 106)";
    else if (grade == 3)
        color = "#3CE6B9";

    QString style = QString("QLabel { color: %1 }").arg(color);
    label->setStyleSheet(style);
    label->setFont(QFont("나눔스퀘어 네오 ExtraBold", 10));
}

// 전설등급 : 0, 유물등급 : 1, 고대등급 : 2, 에스더 : 3
int Profile::getItemGrade(const QJsonObject& obj)
{
    QStringList keys;
    keys << "Element_001" << "value" << "leftStr0";

    QString itemCategory = getValueFromJson(obj, keys).toString();
    if (itemCategory.contains("전설"))
        return 0;
    else if (itemCategory.contains("유물"))
        return 1;
    else if (itemCategory.contains("고대"))
        return 2;
    else if (itemCategory.contains("에스더"))
        return 3;
    else
        return -1;
}

void Profile::clearAll()
{
    // 이전 캐릭터 정보 모두 초기화
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

    mEngraveValue.clear();

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
    ui->leName->clear();

    // 존재하지 않는 캐릭터인 경우 알람 popup 후 종료
    if (profileIndex == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("존재하지 않는 캐릭터입니다.");
        msgBox.exec();
        return;
    }
    else
    {
        if (ui->tabProfile->isHidden())
            ui->tabProfile->show();

        // Profile의 시작 index와 Profile의 크기 계산
        profileIndex += profileStart.size();
        profileSize = responseData.indexOf(profileEnd) - profileIndex + 1;

        // Profile 추출 후 QJSonObject로 변환
        const QString& profile = responseData.sliced(profileIndex, profileSize);
        if (mProfile != nullptr)
            delete mProfile;
        mProfile = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());

        // 추출 결과 parsing
        clearAll();
        parseEquip();
        parseEngrave();
        parseSkill();
        parseCard();
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

    // 해당하는 Label에 아이콘 추가
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

    // 해당하는 Label에 아이콘 추가
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
