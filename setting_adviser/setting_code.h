#ifndef SETTINGCODE_H
#define SETTINGCODE_H

#include "engrave.h"

#include <QString>
#include <QJsonObject>

class SettingCode
{
public:
    static QString generateSettingCode(const QJsonObject& setting);

private:
    static QString generateAbilityCode(const QJsonArray& abilities);
    static QString generateSetEffectCode(const QJsonArray& setEffects);
    static QString generateEngraveCode(const QJsonArray& engraveNames, const QJsonArray& engraveLevels);

    static Engrave m_sEngrave;
};

#endif // SETTINGCODE_H
