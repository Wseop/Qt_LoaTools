#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <QString>
#include <QJsonDocument>

class JsonBuilder
{
public:
    static QJsonDocument buildCharacter(QString name, QString cls, double level);
    static QJsonDocument buildSetting(QString name, QString cls, QStringList abilities, QStringList engraveNames, QList<int> engraveLevels, QStringList setEffects);
};

#endif // JSONBUILDER_H
