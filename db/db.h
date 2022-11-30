#ifndef DB_H
#define DB_H

#include "enum/class.h"
#include "enum/collection.h"

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class DB : public QObject
{
    Q_OBJECT

private:
    DB();
    ~DB();

    void connectDB();
    bsoncxx::builder::stream::document objToDoc(Collection collection, QJsonObject obj);

public:
    static DB* getInstance();
    static void destroyInstance();

public:
    mongocxx::collection getCollection(Collection collection);

signals:
    void requestAllDocuments(Collection collection, int order, QString orderField);
    void requestDocumentsByClass(Collection collection, Class cls, int order, QString orderField);
    void requestDocumentByName(Collection collection, QString name);

    void insertDocument(Collection collection, QJsonObject docObj);
    void updateDocument(Collection collection, QJsonObject docObj);
    void deleteDocument(Collection collection, QString name);

    void finished(QVariantList);
    void finished(QJsonObject);

private slots:
    void slotRequestAllDocuments(Collection collection, int order, QString orderField);
    void slotRequestDocumentsByClass(Collection collection, Class cls, int order, QString orderField);
    void slotRequestDocumentByName(Collection collection, QString name);

    void slotInsertDocument(Collection collection, QJsonObject docObj);
    void slotUpdateDocument(Collection collection, QJsonObject docObj);
    void slotDeleteDocument(Collection collection, QString name);

private:
    mongocxx::instance m_inst{};
    static DB* m_pDb;
    mongocxx::client m_client;
    mongocxx::database m_db;
};

#endif // DB_H
