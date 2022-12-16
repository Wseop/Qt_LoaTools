#include "document_manager.h"

bsoncxx::builder::stream::document DocumentManager::buildDocumentCharacter(QString name, Class cls, double level)
{
    bsoncxx::builder::stream::document doc{};

    doc << "Name" << name.toStdString()
        << "Class" << classToStr(cls).toStdString()
        << "Level" << level;

    return doc;
}

bsoncxx::builder::stream::document DocumentManager::buildDocumentSettingV2(QString name, Class cls, QString settingCode)
{
    bsoncxx::builder::stream::document doc{};

    doc << "Name" << name.toStdString()
        << "Class" << classToStr(cls).toStdString()
        << "SettingCode" << settingCode.toStdString();

    return doc;
}
