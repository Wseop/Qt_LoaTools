#include "db.h"

#include <QSettings>
#include <QJsonDocument>

#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/json.hpp>

using namespace bsoncxx::builder::stream;

DB* DB::m_pDb = nullptr;

DB::DB()
{
    connectDB();
    connect(this, SIGNAL(insertDocument(Collection, QJsonObject)), this, SLOT(slotInsertDocument(Collection, QJsonObject)));
    connect(this, SIGNAL(updateDocument(Collection, QJsonObject)), this, SLOT(slotUpdateDocument(Collection, QJsonObject)));
    connect(this, SIGNAL(deleteDocument(Collection, QString)), this, SLOT(slotDeleteDocument(Collection, QString)));

    connect(this, SIGNAL(requestAllDocuments(Collection, int, QString)), this, SLOT(slotRequestAllDocuments(Collection, int, QString)));
    connect(this, SIGNAL(requestDocumentsByClass(Collection,Class,int,QString)), this, SLOT(slotRequestDocumentsByClass(Collection,Class,int,QString)));
    connect(this, SIGNAL(requestDocumentByName(Collection,QString)), this, SLOT(slotRequestDocumentByName(Collection,QString)));
}

DB::~DB()
{
    destroyInstance();
}

// Connect DB & set collection variables
void DB::connectDB()
{
    QSettings config("../config.ini", QSettings::Format::IniFormat);
    QString userName = config.value("database/user_name").toString();
    QString password = config.value("database/password").toString();

    QString dbUri = QString("mongodb+srv://%1:%2@cluster0.88yln.mongodb.net/?retryWrites=true&w=majority").arg(userName, password);
    m_client = {mongocxx::uri{dbUri.toStdString()}};
    m_db = m_client["LoaTools"];
}

document DB::objToDoc(Collection collection, QJsonObject obj)
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

DB *DB::getInstance()
{
    if (m_pDb == nullptr)
    {
        m_pDb = new DB();
    }
    return m_pDb;
}

void DB::destroyInstance()
{
    if (m_pDb == nullptr)
        return;

    delete m_pDb;
    m_pDb = nullptr;
}

mongocxx::collection DB::getCollection(Collection collection)
{
    switch (collection)
    {
    case Collection::Character:
        return m_db["Character"];
    case Collection::Setting:
        return m_db["Setting"];
    }
}

void DB::slotRequestAllDocuments(Collection collection, int order, QString orderField)
{
    QJsonArray ret;

    auto options = mongocxx::options::find{};
    document orderDoc{};
    if (order == 1 || order == -1)
        orderDoc << orderField.toStdString() << order;
    options.sort(orderDoc.view());

    mongocxx::cursor cursor = getCollection(collection).find({}, options);
    for (auto doc : cursor)
    {
        QString docStr = bsoncxx::to_json(doc).c_str();
        QJsonObject docObj(QJsonDocument::fromJson(docStr.toUtf8()).object());
        ret.append(docObj);
    }

    emit finished(ret.toVariantList());
}

void DB::slotRequestDocumentsByClass(Collection collection, Class cls, int order, QString orderField)
{
    QJsonArray ret;

    auto options = mongocxx::options::find{};
    document orderDoc{};
    if (order == 1 || order == -1)
        orderDoc << orderField.toStdString() << order;
    options.sort(orderDoc.view());

    mongocxx::cursor cursor = getCollection(collection).find(document{} << "Class" << enumClassToStr(cls).toStdString() << finalize, options);
    for (auto doc : cursor)
    {
        QString docStr = bsoncxx::to_json(doc).c_str();
        QJsonObject docObj(QJsonDocument::fromJson(docStr.toUtf8()).object());
        ret.append(docObj);
    }

    emit finished(ret.toVariantList());
}

void DB::slotRequestDocumentByName(Collection collection, QString name)
{
    QJsonObject ret;

    auto doc = getCollection(collection).find_one(document{} << "Name" << name.toStdString() << finalize);
    if (doc)
    {
        QString docStr = bsoncxx::to_json(*doc).c_str();
        ret = QJsonDocument::fromJson(docStr.toUtf8()).object();
    }

    emit finished(ret);
}

void DB::slotInsertDocument(Collection collection, QJsonObject docObj)
{
    // 데이터가 이미 존재하면 update, 없으면 insert
    auto result = getCollection(collection).find_one(document{} << "Name" << docObj.find("Name")->toString().toStdString() << finalize);
    if (result)
    {
        bsoncxx::document::value filter = document{}
                << "Name" << docObj.find("Name")->toString().toStdString() << finalize;
        getCollection(collection).update_one(filter.view(),
                                             document{} << "$set" << objToDoc(collection, docObj) << finalize);
    }
    else
    {
        getCollection(collection).insert_one(objToDoc(collection, docObj).view());
    }
}

void DB::slotUpdateDocument(Collection collection, QJsonObject docObj)
{
    // update document by name
    bsoncxx::document::value filter = document{}
            << "Name" << docObj.find("Name")->toString().toStdString() << finalize;

    getCollection(collection).update_one(filter.view(),
                                         document{} << "$set" << objToDoc(collection, docObj) << finalize);
}

void DB::slotDeleteDocument(Collection collection, QString name)
{
    // delete document by name
    getCollection(collection).delete_one(document{} << "Name" << name.toStdString() << finalize);
}
