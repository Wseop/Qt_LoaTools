#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <QString>
#include <QJsonDocument>

class JsonBuilder
{
public:
    static QJsonDocument buildCharacter(QString name, QString cls, double level);
    static QJsonDocument buildSetting(QString name, QString cls, QStringList abilities, QStringList engraveNames, QList<int> engraveLevels, QStringList setEffects);
    static QJsonDocument buildSettingV2(QString name, QString cls, QString settingCode);
};

#endif // JSONBUILDER_H
