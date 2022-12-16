#ifndef SETTINGCODEMANAGER_H
#define SETTINGCODEMANAGER_H

#include "game_data/profile/enum/ability.h"
#include "game_data/item/enum/set_effect.h"

#include <QObject>

class SettingCodeManager : public QObject
{
    Q_OBJECT

public:
    static QString generateSettingCode(const QList<Ability>& abilities, const QList<SetEffect>& setEffects, const QList<QPair<QString, int>>& engraves);
    static QList<Ability> parseAbilities(QString settingCode);
    static QList<SetEffect> parseSetEffects(QString settingCode);
    static QList<QPair<QString, int>> parseClassEngraves(QString settingCode);
    static QList<QPair<QString, int>> parseNormalEngraves(QString settingCode);

private:
    static QString generateAbilityCode(const QList<Ability>& abilities);
    static QString generateSetEffectCode(const QList<SetEffect>& setEffects);
    static QString generateEngraveCode(const QList<QPair<QString, int>>& engraves);
};

#endif // SETTINGCODEMANAGER_H
