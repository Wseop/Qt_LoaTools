#include "http_client.h"
#include "db/db.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/collection.hpp>

#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

HttpClient* HttpClient::m_pHttpClient = nullptr;

HttpClient::HttpClient()
{
    loadApiKey();
}

HttpClient::~HttpClient()
{
    destroyInstance();
}

void HttpClient::loadApiKey()
{
    DB::getInstance()->lock();

    auto doc = DB::getInstance()->getCollection(Collection::LostarkAPI).find_one(bsoncxx::builder::stream::document{} << "Type" << "ApiKey" << bsoncxx::builder::stream::finalize);
    if (doc)
    {
        QString docStr = bsoncxx::to_json(*doc).c_str();
        QJsonObject docObj(QJsonDocument::fromJson(docStr.toUtf8()).object());
        m_apiKey = docObj.find("Key")->toString();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << ": ApiKey load fail";
    }

    DB::getInstance()->unlock();
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

void HttpClient::sendRequest(QNetworkAccessManager* networkManager, const QString& url)
{
    QNetworkRequest request;
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("authorization", QString("bearer %1").arg(m_apiKey).toUtf8());
    request.setUrl(QUrl(url));
    networkManager->get(request);
}
