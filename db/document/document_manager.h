#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include "game_data/profile/enum/class.h"
#include <bsoncxx/builder/stream/document.hpp>

#include <QObject>

class DocumentManager : public QObject
{
    Q_OBJECT

public:
    static bsoncxx::builder::stream::document buildDocumentCharacter(QString name, Class cls, double level);
    static bsoncxx::builder::stream::document buildDocumentSettingV2(QString name, Class cls, QString settingCode);
};

#endif // DOCUMENTMANAGER_H
