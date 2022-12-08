#ifndef DBREQUEST_H
#define DBREQUEST_H

#include "game_data/profile/enum/class.h"
#include "enum/collection.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>

#include <QObject>
#include <QJsonObject>

class DBRequest : public QObject
{
    Q_OBJECT
public:
    DBRequest();

private:
    bsoncxx::builder::stream::document objToDoc(Collection collection, QJsonObject obj);

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

};

#endif // DBREQUEST_H
