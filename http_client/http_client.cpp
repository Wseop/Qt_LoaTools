#include "http_client.h"
#include "http_client/json_builder.h"

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

    connect(&m_networkCharacter, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotHandleReply(QNetworkReply*)));
    connect(&m_networkSetting, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotHandleReply(QNetworkReply*)));
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

void HttpClient::slotInsertOrUpdateCharacter(QJsonDocument jsonDoc)
{
    QString url = m_serverUrl + "/character";
    QNetworkRequest request((QUrl(url)));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader(m_insertKey.toUtf8(), m_insertValue.toUtf8());
    m_networkCharacter.post(request, jsonDoc.toJson());
}

void HttpClient::slotInsertOrUpdateSetting(QJsonDocument jsonDoc)
{
    QString url = m_serverUrl + "/setting";
    QNetworkRequest request((QUrl(url)));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader(m_insertKey.toUtf8(), m_insertValue.toUtf8());
    m_networkSetting.post(request, jsonDoc.toJson());
}

void HttpClient::slotHandleReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString log = QString("%1 : Status %2").arg(reply->url().path()).arg(statusCode);
    qDebug() << Q_FUNC_INFO << log;
}
