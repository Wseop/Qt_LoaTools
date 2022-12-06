#include "setting_adviser.h"
#include "ui_setting_adviser.h"
#include "class_selector.h"
#include "setting_code.h"
#include "game_data/profile/class.h"
#include "game_data/profile/engrave.h"
#include "ui/setting_widget.h"
#include "db/db_request.h"
#include "font/font_manager.h"

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QPair>
#include <QThread>

#include <algorithm>

SettingAdviser* SettingAdviser::m_pSettingAdviser = nullptr;

SettingAdviser::SettingAdviser() :
    ui(new Ui::SettingAdviser),
    m_pDBRequest(new DBRequest()),
    m_pClassSelector(new ClassSelector(this, ParentClass::SettingAdviser))
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("직업별 세팅 조회");
    this->showMaximized();

    initFont();
    initAlignment();
    initConnect();

    QThread* dbThread = new QThread();
    m_pDBRequest->moveToThread(dbThread);
    dbThread->start();
}

SettingAdviser::~SettingAdviser()
{
    delete m_pClassSelector;
    delete ui;
    destroyInstance();
}

void SettingAdviser::setSelectedClass(int cls)
{
    m_selectedClass = cls;

    ui->lbSelectedClass->clear();
    ui->lbTotal->clear();
    ui->lbInfo->setText("데이터 처리중입니다. 잠시만 기다려주세요...");

    requestSettingsBySelectedClass();
}

SettingAdviser *SettingAdviser::getInstance()
{
    if (m_pSettingAdviser == nullptr)
        m_pSettingAdviser = new SettingAdviser();

    return m_pSettingAdviser;
}

void SettingAdviser::destroyInstance()
{
    if (m_pSettingAdviser == nullptr)
        return;

    delete m_pSettingAdviser;
    m_pSettingAdviser = nullptr;
}

void SettingAdviser::initFont()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumExtraBold12 = fontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 12);
    QFont fontNanumExtraBold16 = fontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 16);
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->lbInfo->setFont(fontNanumExtraBold12);
    ui->lbSelectedClass->setFont(fontNanumExtraBold12);
    ui->lbTotal->setFont(fontNanumExtraBold12);
    ui->groupSetting1->setFont(fontNanumExtraBold16);
    ui->groupSetting2->setFont(fontNanumExtraBold16);
    ui->groupSetting3->setFont(fontNanumExtraBold16);
    ui->pbSelectClass->setFont(fontNanumBold10);
    ui->pbTopSetting->setFont(fontNanumBold10);
}

void SettingAdviser::initAlignment()
{
    ui->hLayoutTop->setAlignment(Qt::AlignLeft);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting1->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting2->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting3->setAlignment(Qt::AlignTop);
}

void SettingAdviser::initConnect()
{
    connect(ui->pbSelectClass, SIGNAL(pressed()), this, SLOT(slotShowClassSelector()));
    connect(ui->pbTopSetting, SIGNAL(pressed()), this, SLOT(slotRequestAllSettings()));
}

void SettingAdviser::clearData()
{
    // 기존 data clear
    QList<SettingWidget*> widgets = m_mapSettingWidgetLayout.keys();
    for (SettingWidget* widget : widgets)
    {
        m_mapSettingWidgetLayout[widget]->removeWidget(widget);
        delete widget;
    }
    m_mapSettingWidgetLayout.clear();
}

void SettingAdviser::requestSettingsBySelectedClass()
{
    connect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleSettingsBySelectedClass(QVariantList)));
    emit m_pDBRequest->requestDocumentsByClass(Collection::Setting, m_selectedClass, 0, "");
}

void SettingAdviser::slotRequestAllSettings()
{
    ui->pbTopSetting->setDisabled(true);
    ui->pbSelectClass->setDisabled(true);
    ui->lbSelectedClass->clear();
    ui->lbTotal->clear();
    ui->lbInfo->setText("데이터 처리중입니다. 잠시만 기다려주세요...");

    connect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleAllSettings(QVariantList)));
    emit m_pDBRequest->requestAllDocuments(Collection::Setting, 0, "");
}

void SettingAdviser::slotHandleSettingsBySelectedClass(QVariantList jsonSettings)
{
    disconnect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleSettingsBySelectedClass(QVariantList)));

    m_numOfCharacters = jsonSettings.size();
    // setting code 생성 및 추가
    m_settingCodes.clear();
    QMap<QString, int> settingCodeCount;
    for (const QVariant& jsonValue : jsonSettings)
    {
        QString settingCode = SettingCode::generateSettingCode(jsonValue.toJsonObject());
        if (settingCode.contains("-1"))
            continue;
        settingCodeCount[settingCode] += 1;
    }

    // value 기준 내림차순 정렬
    m_settingCodes = QList<SettingCodeCount>(settingCodeCount.keyValueBegin(), settingCodeCount.keyValueEnd());
    std::sort(m_settingCodes.begin(), m_settingCodes.end(), [](SettingCodeCount a, SettingCodeCount b) {
        return a.second > b.second;
    });

    renderSettings();
}

void SettingAdviser::renderSettings()
{
    clearData();

    ui->lbSelectedClass->setText(Class::enumToKstr(m_selectedClass));
    QString totalText = QString("등록된 캐릭터 수(1600레벨 이상) : %1 캐릭터").arg(m_numOfCharacters);
    ui->lbTotal->setText(totalText);
    ui->lbInfo->setText("(직업 각인 별 최대 10개의 세팅이 표기되며, 카던 및 pvp 세팅이 포함되어 있을 수 있습니다.)");

    ui->groupSetting3->setTitle("쌍직각");

    int layoutCount[3] = {0, 0, 0};
    for (int i = 0; i < m_settingCodes.size(); i++)
    {
        QString settingCode = m_settingCodes[i].first;

        QStringList abilities = SettingCode::getAbility(settingCode);
        QStringList setEffects = SettingCode::getSetEffect(settingCode);
        QList<QPair<QString, int>> classEngraves = SettingCode::getClassEngrave(settingCode);
        QList<QPair<QString, int>> normalEngraves = SettingCode::getNormalEngrave(settingCode);

        if (classEngraves.size() == 0)
            continue;

        // 직업 각인에 따라, 추가할 layout 지정
        QVBoxLayout* layout = nullptr;
        int layoutIndex = 0;
        if (classEngraves.size() == 2)
        {
            layout = ui->vLayoutSetting3;
            layoutIndex = 2;
        }
        else if (Engrave::getInstance()->indexOf(classEngraves[0].first) & 1)
        {
            layout = ui->vLayoutSetting1;
            layoutIndex = 0;
            ui->groupSetting1->setTitle(classEngraves[0].first);
        }
        else
        {
            layout = ui->vLayoutSetting2;
            layoutIndex = 1;
            ui->groupSetting2->setTitle(classEngraves[0].first);
        }
        layoutCount[layoutIndex]++;

        if (layoutCount[0] > 10 && layoutCount[1] > 10 && layoutCount[2] > 10)
            break;

        if (layoutCount[layoutIndex] <= 10)
        {
            SettingWidget* widget = new SettingWidget();
            widget->setIndex(layoutCount[layoutIndex]);
            widget->setNumOfCharacters(m_settingCodes[i].second);
            widget->setAdoptRatio(((double)m_settingCodes[i].second / m_numOfCharacters));
            widget->setAbilities(abilities);
            widget->setSetEffects(setEffects);
            widget->setClassEngraves(classEngraves);
            widget->setNormalEngraves(normalEngraves);
            layout->addWidget(widget);
            m_mapSettingWidgetLayout[widget] = layout;
        }
    }
}

void SettingAdviser::renderTopSettings()
{
    clearData();

    ui->lbTotal->clear();
    ui->lbInfo->clear();
    ui->groupSetting1->setTitle("1순위");
    ui->groupSetting2->setTitle("2순위");
    ui->groupSetting3->setTitle("3순위");

    for (int i = Class::start(); i <= Class::size(); i++)
    {
        const QList<SettingCodeCount> topSettingCodes = m_topSettingCodes[i];

        for (int j = 0; j < 3; j++)
        {
            QString settingCode = topSettingCodes[j].first;
            double adoptRatio = (double)topSettingCodes[j].second / m_classCounts[i];
            QString title = Class::enumToKstr(i) + QString(" (%1순위 - 채용률 %2%)").arg(j + 1).arg(adoptRatio * 100, 0, 'f', 2, QChar(' '));
            QStringList abilities = SettingCode::getAbility(settingCode);
            QStringList setEffects = SettingCode::getSetEffect(settingCode);
            QList<QPair<QString, int>> classEngraves = SettingCode::getClassEngrave(settingCode);
            QList<QPair<QString, int>> normalEngraves = SettingCode::getNormalEngrave(settingCode);

            QVBoxLayout* layout = nullptr;
            if (j == 0)
                layout = ui->vLayoutSetting1;
            else if (j == 1)
                layout = ui->vLayoutSetting2;
            else
                layout = ui->vLayoutSetting3;

            SettingWidget* widget = new SettingWidget();
            widget->setTitle(title);
            widget->setAbilities(abilities);
            widget->setSetEffects(setEffects);
            widget->setClassEngraves(classEngraves);
            widget->setNormalEngraves(normalEngraves);
            layout->addWidget(widget);
            m_mapSettingWidgetLayout[widget] = layout;
        }
    }
}

void SettingAdviser::slotShowClassSelector()
{
    this->setDisabled(true);
    m_pClassSelector->show();
}

void SettingAdviser::slotHandleAllSettings(QVariantList jsonSettings)
{
    disconnect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleAllSettings(QVariantList)));

    m_topSettingCodes.clear();
    m_topSettingCodes = QList<QList<SettingCodeCount>>(Class::size() + 1);
    m_classCounts.clear();
    m_classCounts = QList<int>(Class::size() + 1);

    QList<QMap<QString, int>> settingCodeCounts(Class::size() + 1);
    for (const QVariant& jsonValue : jsonSettings)
    {
        QString settingCode = SettingCode::generateSettingCode(jsonValue.toJsonObject());
        if (settingCode.contains("-1"))
            continue;

        int cls = Class::eStrToEnum(jsonValue.toJsonObject().find("Class")->toString());
        settingCodeCounts[cls][settingCode] += 1;
        m_classCounts[cls] += 1;
    }

    QList<QList<SettingCodeCount>> topSettingCodes(Class::size() + 1);
    for (int i = Class::start(); i <= Class::size(); i++)
    {
        auto& topSettingCode = topSettingCodes[i];
        topSettingCode = QList<SettingCodeCount>(settingCodeCounts[i].keyValueBegin(), settingCodeCounts[i].keyValueEnd());
        std::sort(topSettingCode.begin(), topSettingCode.end(), [](SettingCodeCount a, SettingCodeCount b) {
            return a.second > b.second;
        });

        for (int j = 0; j < 3; j++)
            m_topSettingCodes[i].append(topSettingCode[j]);
    }

    ui->pbTopSetting->setEnabled(true);
    ui->pbSelectClass->setEnabled(true);
    renderTopSettings();
}
