#include "json_builder.h"

#include <QJsonArray>
#include <QJsonObject>

QJsonDocument JsonBuilder::buildCharacter(QString name, QString cls, double level)
{
    QJsonObject jsonObj;

    jsonObj.insert("Name", name);
    jsonObj.insert("Class", cls);
    jsonObj.insert("Level", level);

    return QJsonDocument(jsonObj);
}

QJsonDocument JsonBuilder::buildSetting(QString name, QString cls, QStringList abilities, QStringList engraveNames, QList<int> engraveLevels, QStringList setEffects)
{
    QJsonObject jsonObj;

    jsonObj.insert("Name", name);
    jsonObj.insert("Class", cls);

    QJsonArray abilitiesArr;
    for (const QString& ability : abilities)
    {
        abilitiesArr.append(ability);
    }
    jsonObj.insert("Abilities", abilitiesArr);

    QJsonArray engraveNamesArr;
    for (const QString& engraveName : engraveNames)
    {
        engraveNamesArr.append(engraveName);
    }
    jsonObj.insert("EngraveNames", engraveNamesArr);

    QJsonArray engraveLevelsArr;
    for (const int& engraveLevel : engraveLevels)
    {
        engraveLevelsArr.append(engraveLevel);
    }
    jsonObj.insert("EngraveLevels", engraveLevelsArr);

    QJsonArray setEffectsArr;
    for (const QString& setEffect: setEffects)
    {
        setEffectsArr.append(setEffect);
    }
    jsonObj.insert("SetEffects", setEffectsArr);

    return QJsonDocument(jsonObj);
}
