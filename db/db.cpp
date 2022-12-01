#include "db.h"
#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>

#include <QSettings>

DB* DB::m_pDb = nullptr;

DB::DB()
{
    connectDB();
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

void DB::lock()
{
    m_mutex.lock();
}

void DB::unlock()
{
    m_mutex.unlock();
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



