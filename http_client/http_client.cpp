#include "http_client.h"

#include <QSettings>
#include <QDir>
#include <QUrl>
#include <QNetworkRequest>

HttpClient* HttpClient::m_pClient = nullptr;

HttpClient::HttpClient()
{
    loadConfig();

    connect(this, SIGNAL(insertOrUpdateCharacter(QJsonDocument)), this, SLOT(slotInsertOrUpdateCharacter(QJsonDocument)));
    connect(this, SIGNAL(insertOrUpdateSetting(QJsonDocument)), this, SLOT(slotInsertOrUpdateSetting(QJsonDocument)));

    connect(this, SIGNAL(readCharacters()), this, SLOT(slotReadCharacters()));
    connect(this, SIGNAL(readCharactersByClass(QString)), this, SLOT(slotReadCharactersByClass(QString)));
    connect(this, SIGNAL(readCharacterByName(QString)), this, SLOT(slotReadCharacterByName(QString)));

    connect(this, SIGNAL(readSettings()), this, SLOT(slotReadSettings()));
    connect(this, SIGNAL(readSettingsByClass(QString)), this, SLOT(slotReadSettingsByClass(QString)));
    connect(this, SIGNAL(readSettingByName(QString)), this, SLOT(slotReadSettingByName(QString)));

    connect(&m_postManagerCharacter, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotHandleReplyPost(QNetworkReply*)));
    connect(&m_postManagerSetting, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotHandleReplyPost(QNetworkReply*)));
}

HttpClient::~HttpClient()
{
    destroyInstance();
}

void HttpClient::loadConfig()
{
    QSettings config(QDir::currentPath() + "/../config.ini", QSettings::Format::IniFormat);
    m_serverUrl = config.value("network/url").toString();
    m_insertKey = config.value("database/insert_key").toString();
    m_insertValue = config.value("database/insert_value").toString();
}

HttpClient *HttpClient::getInstance()
{
    if (m_pClient == nullptr)
    {
        m_pClient = new HttpClient();
    }
    return m_pClient;
}

void HttpClient::destroyInstance()
{
    if (m_pClient == nullptr)
        return;

    delete m_pClient;
    m_pClient = nullptr;
}

const QNetworkAccessManager &HttpClient::getNetworkManagerCharacter()
{
    return m_getManagerCharacter;
}

const QNetworkAccessManager &HttpClient::getNetworkManagerSetting()
{
    return m_getManagerSetting;
}

void HttpClient::slotInsertOrUpdateCharacter(QJsonDocument jsonDoc)
{
    QString url = m_serverUrl + "/character";
    QNetworkRequest request((QUrl(url)));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader(m_insertKey.toUtf8(), m_insertValue.toUtf8());
    m_postManagerCharacter.post(request, jsonDoc.toJson());
}

void HttpClient::slotInsertOrUpdateSetting(QJsonDocument jsonDoc)
{
    QString url = m_serverUrl + "/setting";
    QNetworkRequest request((QUrl(url)));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader(m_insertKey.toUtf8(), m_insertValue.toUtf8());
    m_postManagerSetting.post(request, jsonDoc.toJson());
}

void HttpClient::slotReadCharacters()
{
    QString url = m_serverUrl + "/character";
    QNetworkRequest request((QUrl(url)));
    m_getManagerCharacter.get(request);
}

void HttpClient::slotReadCharactersByClass(QString cls)
{
    QString url = m_serverUrl + "/character/Class-" + cls;
    QNetworkRequest request((QUrl(url)));
    m_getManagerCharacter.get(request);
}

void HttpClient::slotReadCharacterByName(QString name)
{
    QString url = m_serverUrl + "/character/" + name;
    QNetworkRequest request((QUrl(url)));
    m_getManagerCharacter.get(request);
}

void HttpClient::slotReadSettings()
{
    QString url = m_serverUrl + "/setting";
    QNetworkRequest request((QUrl(url)));
    m_getManagerSetting.get(request);
}

void HttpClient::slotReadSettingsByClass(QString cls)
{
    QString url = m_serverUrl + "/setting/Class-" + cls;
    QNetworkRequest request((QUrl(url)));
    m_getManagerSetting.get(request);
}

void HttpClient::slotReadSettingByName(QString name)
{
    QString url = m_serverUrl + "/setting/" + name;
    QNetworkRequest request((QUrl(url)));
    m_getManagerSetting.get(request);
}

void HttpClient::slotHandleReplyPost(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString log = QString("%1 : Status %2").arg(reply->url().path()).arg(statusCode);
    qDebug() << Q_FUNC_INFO << log;
}
