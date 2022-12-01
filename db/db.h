#ifndef DB_H
#define DB_H

#include "enum/collection.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <QObject>

class DB : public QObject
{
    Q_OBJECT

private:
    DB();
    ~DB();

    void connectDB();

public:
    mongocxx::collection getCollection(Collection collection);

public:
    static DB* getInstance();
    static void destroyInstance();   

private:
    mongocxx::instance m_inst{};
    static DB* m_pDb;
    mongocxx::client m_client;
    mongocxx::database m_db;
};

#endif // DB_H
