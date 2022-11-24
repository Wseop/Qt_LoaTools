#include "setting_adviser.h"
#include "ui_setting_adviser.h"
#include "http_client/http_client.h"
#include "class_selector.h"
#include "setting_code.h"
#include "enum/class.h"
#include "ui/setting_widget.h"

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QPair>

#include <algorithm>

SettingAdviser::SettingAdviser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingAdviser),
    m_pClassSelector(new ClassSelector(nullptr, this))
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("직업별 세팅 조회");
    this->showMaximized();

    ui->hLayoutTop->setAlignment(Qt::AlignLeft);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting1->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting2->setAlignment(Qt::AlignTop);
    ui->vLayoutSetting3->setAlignment(Qt::AlignTop);

    initConnect();
}

SettingAdviser::~SettingAdviser()
{
    delete m_pClassSelector;
    delete ui;
}

void SettingAdviser::setSelectedClass(QString cls)
{
    QString classStr = enumClassKtoE(cls);

    m_selectedClass = strToEnumClass(classStr);
    ui->lbSelectedClass->setText(cls);
    readSettingsByClass(classStr);
}

void SettingAdviser::initConnect()
{
    connect(ui->pbSelectClass, SIGNAL(pressed()), this, SLOT(slotShowClassSelector()));
    connect(&HttpClient::getInstance()->getNetworkManagerSetting(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotHandleReplySetSettings(QNetworkReply*)));
}

void SettingAdviser::readSettingsByClass(QString cls)
{
    emit HttpClient::getInstance()->readSettingsByClass(cls);
}

void SettingAdviser::renderSettings()
{
    // 기존 data clear
    QList<SettingWidget*> widgets = m_mapSettingWidgetLayout.keys();
    for (SettingWidget* widget : widgets)
    {
        m_mapSettingWidgetLayout[widget]->removeWidget(widget);
        delete widget;
    }
    m_mapSettingWidgetLayout.clear();

    QString totalText = QString("등록된 캐릭터 수(1600레벨 이상) : %1캐릭터").arg(m_numOfCharacters);
    ui->lbTotal->setText(totalText);
    ui->lbInfo->setText("(직업 각인 별 최대 10개의 세팅이 표기되며, 카던 및 pvp 세팅이 포함되어 있을 수 있습니다.)");

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
        else if (m_engrave.indexOf(classEngraves[0].first) & 1)
        {
            layout = ui->vLayoutSetting1;
            layoutIndex = 0;
        }
        else
        {
            layout = ui->vLayoutSetting2;
            layoutIndex = 1;
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

void SettingAdviser::slotShowClassSelector()
{
    this->setDisabled(true);
    m_pClassSelector->show();
}

void SettingAdviser::slotHandleReplySetSettings(QNetworkReply* reply)
{
    QJsonArray jsonSettings = QJsonDocument::fromJson(reply->readAll()).array();

    m_numOfCharacters = jsonSettings.size();
    // setting code 생성 및 추가
    m_settingCodes.clear();
    QMap<QString, int> settingCodeCount;
    for (const QJsonValue& jsonValue : jsonSettings)
    {
        QString settingCode = SettingCode::generateSettingCode(jsonValue.toObject());
        settingCodeCount[settingCode] += 1;
    }

    // value 기준 내림차순 정렬
    m_settingCodes = QList<QPair<QString, int>>(settingCodeCount.keyValueBegin(), settingCodeCount.keyValueEnd());
    std::sort(m_settingCodes.begin(), m_settingCodes.end(), [](QPair<QString, int> a, QPair<QString, int> b) {
        return a.second > b.second;
    });

    renderSettings();
}
