#include "http_client.h"
#include "db/db.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/collection.hpp>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HttpClient* HttpClient::m_pHttpClient = nullptr;

HttpClient::HttpClient() :
    m_pDB(DB::getInstance())
{
    loadApi();
    loadApiKeys();
}

HttpClient::~HttpClient()
{
    destroyInstance();
}

void HttpClient::loadApi()
{
    QString filePath = QDir::currentPath() + "/resources/api.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << ": File open fail";
        return;
    }

    QTextStream inStream(&file);
    while (!inStream.atEnd())
        m_apis << inStream.readLine();

    file.close();
}

void HttpClient::loadApiKeys()
{
    m_pDB->lock();

    auto doc = m_pDB->getCollection(Collection::LostarkAPI).find_one(bsoncxx::builder::stream::document{} << "Type" << "ApiKeys" << bsoncxx::builder::stream::finalize);
    if (doc)
    {
        QJsonObject docObj = QJsonDocument::fromJson(bsoncxx::to_json(*doc).data()).object();
        const QJsonArray& keys = docObj.find("Keys")->toArray();
        for (const QJsonValue& key : keys)
        {
            m_apiKeys << key.toString();
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO << ": ApiKeys load fail";
    }

    m_pDB->unlock();
}

HttpClient* HttpClient::getInstance()
{
    if (m_pHttpClient == nullptr)
        m_pHttpClient = new HttpClient();

    return m_pHttpClient;
}

void HttpClient::destroyInstance()
{
    if (m_pHttpClient == nullptr)
        return;

    delete m_pHttpClient;
    m_pHttpClient = nullptr;
}

void HttpClient::sendGetRequest(QNetworkAccessManager* pNetworkManager, LostarkApi api, int keyIndex, const QStringList& params)
{
    if (keyIndex >= MAX_APIKEY)
    {
        qDebug() << Q_FUNC_INFO << ": Invalid key index";
        return;
    }

    // generate url
    QString url = m_apis[static_cast<int>(api)];
    for (const QString& param : params)
        url = url.arg(param);

    // generate & send request
    QNetworkRequest request;
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("authorization", QString("bearer %1").arg(m_apiKeys[keyIndex]).toUtf8());
    request.setUrl(QUrl(url));
    pNetworkManager->get(request);
}
