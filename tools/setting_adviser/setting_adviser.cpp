#include "setting_adviser.h"
#include "ui_setting_adviser.h"
#include "class_selector.h"
#include "game_data/engrave/engrave_manager.h"
#include "db/db_request.h"
#include "font/font_manager.h"
#include "tools/setting_adviser/ui/setting_widget.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

#include <algorithm>

void loadSettings(DBRequest* pDbRequest, Class selectedClass)
{
    pDbRequest->findDocuments(Collection::SettingV2, 0, "", "Class", classToStr(selectedClass));
}

SettingAdviser* SettingAdviser::m_pSettingAdviser = nullptr;

SettingAdviser::SettingAdviser() :
    ui(new Ui::SettingAdviser),
    m_pDBRequest(new DBRequest()),
    m_pClassSelector(new ClassSelector(this, ParentClass::SettingAdviser)),
    m_selectedClass(Class::Size)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("직업별 세팅 조회");
    this->showMaximized();

    setConnects();
    setFonts();
    setAlignments();
}

SettingAdviser::~SettingAdviser()
{
    delete m_pClassSelector;
    delete ui;
    destroyInstance();
}

void SettingAdviser::setConnects()
{
    connect(ui->pbSelectClass, &QPushButton::pressed, this, [&](){
        m_pClassSelector->show();
    });
    connect(m_pDBRequest, &DBRequest::findDocumentsFinished, this, &SettingAdviser::setSettings);
}

void SettingAdviser::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->pbSelectClass->setFont(nanumBold10);
    ui->lbSelectedClass->setFont(nanumBold10);
    ui->lbTotal->setFont(nanumBold10);
    ui->lbInfo->setFont(nanumBold10);
}

void SettingAdviser::setAlignments()
{
    ui->vLayoutScrollArea->setAlignment(Qt::AlignHCenter);
}

void SettingAdviser::clearData()
{
    m_settingCounts.clear();
    for (SettingWidget* pSettingWidget : m_settingWidgets)
        delete pSettingWidget;
    m_settingWidgets.clear();
}

void SettingAdviser::setSettings(QJsonArray settings)
{
    clearData();
    m_settings = settings;
    setSettingCounts();
}

void SettingAdviser::setSettingCounts()
{
    QMap<QString, int> settingToCount;

    for (const QJsonValue& setting : m_settings)
    {
        settingToCount[setting.toObject().find("SettingCode")->toString()]++;
    }

    m_settingCounts = QList<QPair<QString, int>>(settingToCount.keyValueBegin(), settingToCount.keyValueEnd());
    std::sort(m_settingCounts.begin(), m_settingCounts.end(), [](QPair<QString, int> a, QPair<QString, int> b){
        return a.second > b.second;
    });

    renderSettings();
}

void SettingAdviser::renderSettings()
{
    ui->lbSelectedClass->setText(classToStr(m_selectedClass));
    ui->lbTotal->setText(QString("등록된 캐릭터 수 : %1개 캐릭터").arg(m_settings.size()));
    ui->lbInfo->setText("카던 및 pvp세팅이 포함되어 있을 수 있습니다.");

    for (int i = 0; i < RENDER_COUNT; i++)
    {
        const auto& settingCount = m_settingCounts[i];
        int characterCount = settingCount.second;
        double adoptRate = ((double)characterCount / m_settings.size()) * 100;

        SettingWidget* pSettingWidget = createSettingWidget(settingCount.first, i + 1, adoptRate, characterCount);
        ui->vLayoutScrollArea->addWidget(pSettingWidget);
    }
}

SettingWidget* SettingAdviser::createSettingWidget(QString settingCode, int rank, double adoptRate, int characterCount)
{
    SettingWidget* pSettingWidget = new SettingWidget(settingCode, rank, adoptRate, characterCount);
    m_settingWidgets.append(pSettingWidget);
    return pSettingWidget;
}

void SettingAdviser::setSelectedClass(Class cls)
{
    m_selectedClass = cls;

    ui->lbSelectedClass->clear();
    ui->lbTotal->clear();
    ui->lbInfo->setText("데이터 처리중입니다. 잠시만 기다려주세요...");

    QThread* thread = QThread::create(loadSettings, m_pDBRequest, m_selectedClass);
    thread->start();
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
