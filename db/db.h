#ifndef DB_H
#define DB_H

#include "enum/collection.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <QObject>
#include <QMutex>

class DB : public QObject
{
    Q_OBJECT

private:
    DB();
    ~DB();

    void connectDB();

public:
    mongocxx::collection getCollection(Collection collection);
    void lock();
    void unlock();

public:
    static DB* getInstance();
    static void destroyInstance();   

private:
    mongocxx::instance m_inst{};
    static DB* m_pDb;
    mongocxx::client m_client;
    mongocxx::database m_db;

    QMutex m_mutex;
};

#endif // DB_H
