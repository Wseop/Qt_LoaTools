#include "setting_adviser.h"
#include "ui_setting_adviser.h"
#include "http_client/http_client.h"
#include "class_selector.h"
#include "setting_code.h"
#include "enum/class.h"

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>

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

void SettingAdviser::slotShowClassSelector()
{
    this->setDisabled(true);
    m_pClassSelector->show();
}

void SettingAdviser::slotHandleReplySetSettings(QNetworkReply* reply)
{
    QJsonArray jsonSettings = QJsonDocument::fromJson(reply->readAll()).array();

    qDebug() << SettingCode::generateSettingCode(jsonSettings[0].toObject());
}
