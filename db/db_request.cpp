#include "db_request.h"
#include "db/db.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/options/find.hpp>

#include <QVariant>
#include <QJsonArray>
#include <QJsonDocument>

using namespace bsoncxx::builder::stream;

DBRequest::DBRequest()
{
    connect(this, SIGNAL(insertDocument(Collection, QJsonObject)), this, SLOT(slotInsertDocument(Collection, QJsonObject)));
    connect(this, SIGNAL(updateDocument(Collection, QJsonObject)), this, SLOT(slotUpdateDocument(Collection, QJsonObject)));
    connect(this, SIGNAL(deleteDocument(Collection, QString)), this, SLOT(slotDeleteDocument(Collection, QString)));

    connect(this, SIGNAL(requestAllDocuments(Collection, int, QString)), this, SLOT(slotRequestAllDocuments(Collection, int, QString)));
    connect(this, SIGNAL(requestDocumentsByClass(Collection,Class,int,QString)), this, SLOT(slotRequestDocumentsByClass(Collection,Class,int,QString)));
    connect(this, SIGNAL(requestDocumentByName(Collection,QString)), this, SLOT(slotRequestDocumentByName(Collection,QString)));
}

document DBRequest::objToDoc(Collection collection, QJsonObject obj)
{
    document doc{};
    QJsonArray jsonArray;

    doc << "Name" << obj.find("Name")->toString().toStdString()
        << "Class" << obj.find("Class")->toString().toStdString();

    if (Collection::Character == collection)
    {
        doc << "Level" << obj.find("Level")->toDouble();
    }
    else if (Collection::Setting == collection)
    {
        auto arrayBuilder = doc << "Abilities" << open_array;
        jsonArray = obj.find("Abilities")->toArray();
        for (const QJsonValue& value : jsonArray)
        {
            arrayBuilder << value.toString().toStdString();
        }
        arrayBuilder << close_array;

        arrayBuilder = doc <<"EngraveNames" << open_array;
        jsonArray = obj.find("EngraveNames")->toArray();
        for (const QJsonValue& value : jsonArray)
        {
            arrayBuilder << value.toString().toStdString();
        }
        arrayBuilder << close_array;

        arrayBuilder = doc << "EngraveLevels" << open_array;
        jsonArray = obj.find("EngraveLevels")->toArray();
        for (const QJsonValue& value : jsonArray)
        {
            arrayBuilder << value.toInt();
        }
        arrayBuilder << close_array;

        arrayBuilder = doc << "SetEffects" << open_array;
        jsonArray = obj.find("SetEffects")->toArray();
        for (const QJsonValue& value : jsonArray)
        {
            arrayBuilder << value.toString().toStdString();
        }
        arrayBuilder << close_array;
    }

    return doc;
}

void DBRequest::slotRequestAllDocuments(Collection collection, int order, QString orderField)
{
    QJsonArray ret;

    auto options = mongocxx::options::find{};
    document orderDoc{};
    if (order == 1 || order == -1)
        orderDoc << orderField.toStdString() << order;
    options.sort(orderDoc.view());

    mongocxx::cursor cursor = DB::getInstance()->getCollection(collection).find({}, options);
    for (auto doc : cursor)
    {
        QString docStr = bsoncxx::to_json(doc).c_str();
        QJsonObject docObj(QJsonDocument::fromJson(docStr.toUtf8()).object());
        ret.append(docObj);
    }

    emit finished(ret.toVariantList());
}

void DBRequest::slotRequestDocumentsByClass(Collection collection, Class cls, int order, QString orderField)
{
    QJsonArray ret;

    auto options = mongocxx::options::find{};
    document orderDoc{};
    if (order == 1 || order == -1)
        orderDoc << orderField.toStdString() << order;
    options.sort(orderDoc.view());

    mongocxx::cursor cursor = DB::getInstance()->getCollection(collection).find(document{} << "Class" << enumClassToStr(cls).toStdString() << finalize, options);
    for (auto doc : cursor)
    {
        QString docStr = bsoncxx::to_json(doc).c_str();
        QJsonObject docObj(QJsonDocument::fromJson(docStr.toUtf8()).object());
        ret.append(docObj);
    }

    emit finished(ret.toVariantList());
}

void DBRequest::slotRequestDocumentByName(Collection collection, QString name)
{
    QJsonObject ret;

    auto doc = DB::getInstance()->getCollection(collection).find_one(document{} << "Name" << name.toStdString() << finalize);
    if (doc)
    {
        QString docStr = bsoncxx::to_json(*doc).c_str();
        ret = QJsonDocument::fromJson(docStr.toUtf8()).object();
    }

    emit finished(ret);
}

void DBRequest::slotInsertDocument(Collection collection, QJsonObject docObj)
{
    // 데이터가 이미 존재하면 update, 없으면 insert
    auto result = DB::getInstance()->getCollection(collection).find_one(document{} << "Name" << docObj.find("Name")->toString().toStdString() << finalize);
    if (result)
    {
        bsoncxx::document::value filter = document{}
                << "Name" << docObj.find("Name")->toString().toStdString() << finalize;
        DB::getInstance()->getCollection(collection).update_one(filter.view(),
                                             document{} << "$set" << objToDoc(collection, docObj) << finalize);
    }
    else
    {
        DB::getInstance()->getCollection(collection).insert_one(objToDoc(collection, docObj).view());
    }
}

void DBRequest::slotUpdateDocument(Collection collection, QJsonObject docObj)
{
    // update document by name
    bsoncxx::document::value filter = document{}
            << "Name" << docObj.find("Name")->toString().toStdString() << finalize;

    DB::getInstance()->getCollection(collection).update_one(filter.view(),
                                         document{} << "$set" << objToDoc(collection, docObj) << finalize);
}

void DBRequest::slotDeleteDocument(Collection collection, QString name)
{
    // delete document by name
    DB::getInstance()->getCollection(collection).delete_one(document{} << "Name" << name.toStdString() << finalize);
}
