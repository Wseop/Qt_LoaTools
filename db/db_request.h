#ifndef DBREQUEST_H
#define DBREQUEST_H

#include "db/db.h"
#include "enum/collection.h"
#include <bsoncxx/builder/stream/document.hpp>

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class DBRequest : public QObject
{
    Q_OBJECT

public:
    DBRequest();

    void findDocument(Collection collection, QString filterField, QString filterValue);
    void findDocuments(Collection collection, int order, QString orderField, QString filterField = "", QString filterValue = "");
    void insertOrUpdateDocument(Collection collection, bsoncxx::document::value doc, QString dupCheckField, QString dupCheckValue);
    void deleteDocument(Collection collection, QString filterField, QString filterValue);

signals:
    void findDocumentFinished(QJsonObject);
    void findDocumentsFinished(QJsonArray);

private:
    DB* m_pDb;
};

#endif // DBREQUEST_H
