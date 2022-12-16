#ifndef DBREQUEST_H
#define DBREQUEST_H

#include "db/db.h"
#include "enum/collection.h"
#include <bsoncxx/builder/stream/document.hpp>

#include <QObject>

class DBRequest : public QObject
{
    Q_OBJECT

public:
    DBRequest();

    void insertOrUpdateDocument(Collection collection, bsoncxx::document::value doc, QString dupCheckField, QString dupCheckValue);

private:
    DB* m_pDb;
};

#endif // DBREQUEST_H
