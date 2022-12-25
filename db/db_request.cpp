#include "db_request.h"

#include <bsoncxx/json.hpp>
#include <mongocxx/cursor.hpp>
#include <mongocxx/options/find.hpp>

#include <QJsonDocument>

using namespace bsoncxx::builder::stream;

DBRequest::DBRequest() :
    m_pDb(DB::getInstance())
{

}

void DBRequest::findDocument(Collection collection, QString filterField, QString filterValue)
{
    m_pDb->lock();

    QJsonObject result;
    auto doc = m_pDb->getCollection(collection).find_one(document{} << filterField.toStdString() << filterValue.toStdString() << finalize);
    if (doc)
    {
        result = QJsonDocument::fromJson(bsoncxx::to_json(*doc).data()).object();
    }

    m_pDb->unlock();

    emit findDocumentFinished(result);
}

void DBRequest::findDocuments(Collection collection, int order, QString orderField, QString filterField, QString filterValue)
{
    // set sort option
    mongocxx::options::find options{};
    document orderDoc{};
    if (order == 1 || order == -1)
        orderDoc << orderField.toStdString() << order;
    options.sort(orderDoc.view());

    // set filter
    document filterDoc{};
    if (filterField != "")
    {
        filterDoc << filterField.toStdString() << filterValue.toStdString();
    }

    m_pDb->lock();

    QJsonArray result;
    mongocxx::cursor cursor = m_pDb->getCollection(collection).find(filterDoc.view(), options);
    for (auto doc : cursor)
    {
        result.append(QJsonDocument::fromJson(bsoncxx::to_json(doc).data()).object());
    }

    m_pDb->unlock();

    emit findDocumentsFinished(result);
}

void DBRequest::insertOrUpdateDocument(Collection collection, bsoncxx::document::value doc, QString dupCheckField, QString dupCheckValue)
{
    document filter{};
    filter << dupCheckField.toStdString() << dupCheckValue.toStdString();

    m_pDb->lock();

    // document 중복 체크
    // 중복이면 update, 아니면 insert
    auto result = m_pDb->getCollection(collection).find_one(filter.view());
    if (result)
    {
        m_pDb->getCollection(collection).update_one(filter.view(), document{} << "$set" << doc << finalize);
    }
    else
    {
        m_pDb->getCollection(collection).insert_one(doc.view());
    }

    m_pDb->unlock();
}

void DBRequest::deleteDocument(Collection collection, QString filterField, QString filterValue)
{
    m_pDb->lock();

    m_pDb->getCollection(collection).delete_one(document{} << filterField.toStdString() << filterValue.toStdString() << finalize);

    m_pDb->unlock();
}
