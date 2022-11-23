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

    for (int i = 0; i < RENDER_COUNT; i++)
    {
        QString settingCode = m_settingCodes[i].first;

        QStringList abilities = SettingCode::getAbility(settingCode);
        QStringList setEffects = SettingCode::getSetEffect(settingCode);
        QList<QPair<QString, int>> classEngraves = SettingCode::getClassEngrave(settingCode);
        QList<QPair<QString, int>> normalEngraves = SettingCode::getNormalEngrave(settingCode);

        // 직업 각인에 따라, 추가할 layout 지정
        QVBoxLayout* layout = nullptr;
        if (classEngraves.size() == 2)
            layout = ui->vLayoutSetting3;
        else if (m_engrave.indexOf(classEngraves[0].first) & 1)
            layout = ui->vLayoutSetting1;
        else
            layout = ui->vLayoutSetting2;

        SettingWidget* widget = new SettingWidget();
        widget->setAbilities(abilities);
        widget->setSetEffects(setEffects);
        widget->setClassEngraves(classEngraves);
        widget->setNormalEngraves(normalEngraves);
        layout->addWidget(widget);
        m_mapSettingWidgetLayout[widget] = layout;
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
