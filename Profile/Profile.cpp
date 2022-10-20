#include "Profile.h"
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

#ifdef QT_DEBUG
    #include <QFile>
#endif

Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile),
    mNetworkProfile(new QNetworkAccessManager()),
    mNetworkIcon(new QNetworkAccessManager()),
    mHtmlTag("<[^>]*>")
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 조회");

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

    mPartQual[Part::WEAPON] = ui->barQualWeapon;
    mPartQual[Part::HEAD] = ui->barQualHead;
    mPartQual[Part::TOP] = ui->barQualTop;
    mPartQual[Part::BOTTOM] = ui->barQualBottom;
    mPartQual[Part::HAND] = ui->barQualHand;
    mPartQual[Part::SHOULDER] = ui->barQualShoulder;

    mPartName[Part::WEAPON] = ui->lbNameWeapon;
    mPartName[Part::HEAD] = ui->lbNameHead;
    mPartName[Part::TOP] = ui->lbNameTop;
    mPartName[Part::BOTTOM] = ui->lbNameBottom;
    mPartName[Part::HAND] = ui->lbNameHand;
    mPartName[Part::SHOULDER] = ui->lbNameShoulder;

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
}

void Profile::initUI()
{
    ui->tabProfile->setFixedHeight(650);

    ui->groupBoxSearch->setFixedHeight(60);
    ui->groupWeapon->setFixedHeight(100);
    ui->groupHead->setFixedHeight(100);
    ui->groupShoulder->setFixedHeight(100);
    ui->groupTop->setFixedHeight(100);
    ui->groupBottom->setFixedHeight(100);
    ui->groupHand->setFixedHeight(100);

    ui->barQualWeapon->setFixedSize(50, 15);
    ui->barQualHead->setFixedSize(50, 15);
    ui->barQualShoulder->setFixedSize(50, 15);
    ui->barQualTop->setFixedSize(50, 15);
    ui->barQualBottom->setFixedSize(50, 15);
    ui->barQualHand->setFixedSize(50, 15);

    ui->tabEquip->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabSkill->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
    ui->tabCard->setStyleSheet("QWidget { background-color: rgb(240, 240, 240) }");
}

void Profile::initConnect()
{
    connect(ui->pbSearch, SIGNAL(pressed()), this, SLOT(slotProfileRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(slotProfileRequest()));
    connect(mNetworkProfile, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotExtractProfile(QNetworkReply*)));
    connect(mNetworkIcon, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotSetIcon(QNetworkReply*)));
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
    mPathPart.clear();

    // Equip key의 값으로 Equip(장비류)와 보석 정보가 들어있음
    QJsonObject equip(mProfile->find("Equip")->toObject());
    QStringList equipKeys;   // 무기, 방어구, 악세 key값
    QStringList jewelKeys;  // 보석 key값

    // key값 추출
    const QStringList& keys = equip.keys();
    for (const QString& key : keys)
    {
        // key값이 GEM을 포함하면 보석류, 아니면 gear류
        if (key.contains("Gem"))
        {
            jewelKeys.append(key);
        }
        else
        {
            equipKeys.append(key);
        }
    }

    // 장비 정보 추출
    for (const QString& key : equipKeys)
    {
        if (key.endsWith("0" + QString::number(static_cast<int>(Part::WEAPON))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HEAD))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::TOP))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::BOTTOM))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::HAND))) ||
            key.endsWith("0" + QString::number(static_cast<int>(Part::SHOULDER))))
        {
            const QJsonObject& obj = equip.find(key)->toObject();

            QStringList nameKeys;
            nameKeys << "Element_000" << "value";
            QStringList levelKeys;
            levelKeys << "Element_001" << "value" << "leftStr2";
            QStringList qualKeys;
            qualKeys << "Element_001" << "value" << "qualityValue";
            QStringList setKeys;
            setKeys << "Element_008" << "value" << "Element_001";
            QStringList iconKeys;
            iconKeys << "Element_001" << "value" << "slotData" << "iconPath";

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

            mPathPart[iconPath] = part;
            requestIcon(iconPath);

            mPartQual[part]->setValue(quality);
            setQualityColor(part, quality);

            mPartName[part]->setText(name);
            setNameColor(part, getItemGrade(obj));

            mPartLevel[part]->setText(level);

            mPartSet[part]->setText(set);
        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::NECK))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::EAR2))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING1))) ||
                 key.endsWith("0" + QString::number(static_cast<int>(Part::RING2))))
        {

        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::STONE))))
        {

        }
        else if (key.endsWith("0" + QString::number(static_cast<int>(Part::BRACELET))))
        {

        }
    }
}

void Profile::parseEngrave()
{}

void Profile::parseSkill()
{}

void Profile::parseCard()
{}

void Profile::requestIcon(QString iconPath)
{
    QNetworkRequest request((QUrl(CDN_PATH + iconPath)));
    mNetworkIcon->get(request);
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

void Profile::setNameColor(Part part, int grade)
{
    QString color;

    if (grade == 0)
        color = "#FA5000";
    else if (grade == 1)
        color = "rgb(210, 166, 106)";

    QString style = QString("QLabel { color: %1; font-size: 10 }").arg(color);
    mPartName[part]->setStyleSheet(style);
    mPartName[part]->setFont(QFont("나눔스퀘어 네오 ExtraBold"));
}

// 유물등급 : 0, 고대등급 : 1
int Profile::getItemGrade(const QJsonObject& obj)
{
    QStringList keys;
    keys << "Element_001" << "value" << "leftStr0";

    QString itemCategory = getValueFromJson(obj, keys).toString();
    if (itemCategory.contains("유물"))
        return 0;
    else if (itemCategory.contains("고대"))
        return 1;
    else
        return -1;
}

void Profile::slotHome()
{
    QWidget* parent = static_cast<QWidget*>(this->parent());
    parent->setFixedWidth(800);
    parent->setFixedHeight(600);

    this->close();
}

void Profile::slotProfileRequest()
{
    // 입력받은 캐릭터명으로 전투정보실에 request를 보냄
    QString name = ui->leName->text();
    QString url = PROFILE_PATH + "/" + name;
    QNetworkRequest request((QUrl(url)));
    mNetworkProfile->get(request);

    // 입력값 clear
    ui->leName->clear();
}

// 응답 결과로부터 Profile(Json형식) 부분 추출
void Profile::slotExtractProfile(QNetworkReply* reply)
{
    const QString profileStart = "$.Profile = ";
    const QString profileEnd = "};";

    QString responseData = reply->readAll();
    qsizetype profileIndex = responseData.indexOf(profileStart);
    qsizetype profileSize = 0;

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
        // Profile의 시작 index와 Profile의 크기 계산
        profileIndex += profileStart.size();
        profileSize = responseData.indexOf(profileEnd) - profileIndex + 1;

        // Profile 추출 후 QJSonObject로 변환
        const QString& profile = responseData.sliced(profileIndex, profileSize);
        if (mProfile != nullptr)
            delete mProfile;
        mProfile = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());

        // 추출 결과 parsing
        parseEquip();
        parseEngrave();
        parseSkill();
        parseCard();
    }
}

void Profile::slotSetIcon(QNetworkReply* reply)
{
    QPixmap icon;
    bool load = icon.loadFromData(reply->readAll(), "PNG");
    if (!load)
    {
        qDebug() << "Icon load fail";
        return;
    }

    // 해당하는 Label에 아이콘 추가
    Part part = mPathPart[reply->url().path()];
    QLabel* iconLabel = mPartIcon[part];

    iconLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(50, 50);
    iconLabel->setStyleSheet("QLabel { border: 1px solid black }");
}
