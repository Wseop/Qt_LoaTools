#include "db_request.h"

using namespace bsoncxx::builder::stream;

DBRequest::DBRequest() :
    m_pDb(DB::getInstance())
{

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
