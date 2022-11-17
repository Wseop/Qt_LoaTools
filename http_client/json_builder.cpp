#include "json_builder.h"

#include <QJsonArray>
#include <QJsonObject>

QJsonDocument JsonBuilder::buildCharacter(QString name, QString cls, double level, const QStringList& setEffects)
{
    QJsonObject jsonObj;

    jsonObj.insert("Name", name);
    jsonObj.insert("Class", cls);
    jsonObj.insert("Level", level);

    QJsonArray setEffectArr;
    for (const QString& setEffect : setEffects)
        setEffectArr.append(setEffect);
    jsonObj.insert("SetEffects", setEffectArr);

    return QJsonDocument(jsonObj);
}

QJsonDocument JsonBuilder::buildAbility(QString name, QString cls, QStringList neck, QStringList ear, QStringList ring)
{
    QJsonObject jsonObj;

    jsonObj.insert("Name", name);
    jsonObj.insert("Class", cls);

    QJsonArray neckArr;
    for (const QString& str : neck)
        neckArr.append(str);
    jsonObj.insert("Neck", neckArr);

    QJsonArray earArr;
    for (const QString& str : ear)
        earArr.append(str);
    jsonObj.insert("Ear", earArr);

    QJsonArray ringArr;
    for (const QString& str : ring)
        ringArr.append(str);
    jsonObj.insert("Ring", ringArr);

    return QJsonDocument(jsonObj);
}

QJsonDocument JsonBuilder::buildEngrave(QString name, QString cls, QStringList engraves, QList<int> levels)
{
    QJsonObject jsonObj;

    jsonObj.insert("Name", name);
    jsonObj.insert("Class", cls);

    QJsonArray engraveArr;
    for (const QString& engrave : engraves)
        engraveArr.append(engrave);
    jsonObj.insert("Engraves", engraveArr);

    QJsonArray levelArr;
    for (const int& level : levels)
        levelArr.append(level);
    jsonObj.insert("Levels", levelArr);

    return QJsonDocument(jsonObj);
}
