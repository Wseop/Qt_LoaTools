#include "setting_code.h"
#include "game_data/profile/enum/ability.h"
#include "game_data/item/enum/set_effect.h"
#include "game_data/engrave/engrave_manager.h"

#include <QJsonArray>
#include <utility>
#include <algorithm>

QString SettingCode::generateSettingCode(const QJsonObject &setting)
{
    return  generateAbilityCode(setting.find("Abilities")->toArray()) +
            generateSetEffectCode(setting.find("SetEffects")->toArray()) +
            generateEngraveCode(setting.find("EngraveNames")->toArray(),
                                setting.find("EngraveLevels")->toArray());
}

QStringList SettingCode::getAbility(const QString &settingCode)
{
    qsizetype startIndex = settingCode.indexOf("A") + 1;
    qsizetype endIndex = settingCode.indexOf("S");
    QString abilityCode = settingCode.sliced(startIndex, endIndex - startIndex);

    QStringList abilities;
    for (const QChar& ability : abilityCode)
        abilities << abilityToStr(static_cast<Ability>(ability.unicode() - '0'));

    return abilities;
}

QStringList SettingCode::getSetEffect(const QString &settingCode)
{
    qsizetype startIndex = settingCode.indexOf("S") + 1;
    qsizetype endIndex = settingCode.indexOf("C");
    QString setEffectCode = settingCode.sliced(startIndex, endIndex - startIndex);

    QList<int> setEffectCounts(static_cast<int>(SetEffect::Size), 0);
    for (const QChar& setEffect : setEffectCode)
        setEffectCounts[setEffect.unicode() - '0']++;

    QStringList setEffects;
    for (int i = 0; i < setEffectCounts.size(); i++)
    {
        int count = setEffectCounts[i];
        if (count == 0)
            continue;

        setEffects << QString("%1%2").arg(count).arg(setEffectToStr(static_cast<SetEffect>(i)));
    }

    return setEffects;
}

// 직업각인과 각인레벨 Map에 mapping 후 레벨을 기준으로 내림차순 정렬하여 반환
QList<QPair<QString, int>> SettingCode::getClassEngrave(const QString &settingCode)
{
    // 직업각인 코드 parsing
    qsizetype startIndex = settingCode.indexOf("C") + 1;
    qsizetype endIndex = settingCode.indexOf("CL");
    QString classEngraveCode = settingCode.sliced(startIndex, endIndex - startIndex);

    // 직업각인레벨 코드 parsing
    startIndex = endIndex + 2;
    endIndex = settingCode.indexOf("N");
    QString classEngraveLevelCode = settingCode.sliced(startIndex, endIndex - startIndex);

    int index = 0;
    QMap<QString, int> classEngraveLevelMap;
    while (index < classEngraveLevelCode.size())
    {
        int engraveCode = classEngraveCode.sliced(index * 3, 3).toInt();
        QString engraveName = EngraveManager::getInstance()->getEngraveByIndex(engraveCode);
        int engraveLevel = classEngraveLevelCode[index].unicode() - '0';

        classEngraveLevelMap[engraveName] = engraveLevel;
        index++;
    }

    QList<QPair<QString, int>> classEngraves(classEngraveLevelMap.keyValueBegin(), classEngraveLevelMap.keyValueEnd());
    std::sort(classEngraves.begin(), classEngraves.end(), [](QPair<QString, int> a, QPair<QString, int> b) {
        return a.second > b.second;
    });

    return classEngraves;
}

// 일반각인과 각인레벨 Map에 mapping 후 레벨을 기준으로 내림차순 정렬하여 반환
QList<QPair<QString, int>> SettingCode::getNormalEngrave(const QString &settingCode)
{
    // 일반각인 코드 parsing
    qsizetype startIndex = settingCode.indexOf("N") + 1;
    qsizetype endIndex = settingCode.indexOf("NL");
    QString normalEngraveCode = settingCode.sliced(startIndex, endIndex - startIndex);

    // 일반각인레벨 코드 parsing
    startIndex = endIndex + 2;
    endIndex = settingCode.size();
    QString normalEngraveLevelCode = settingCode.sliced(startIndex, endIndex - startIndex);

    int index = 0;
    QMap<QString, int> normalEngraveLevelMap;
    while (index < normalEngraveLevelCode.size())
    {
        int engraveCode = normalEngraveCode.sliced(index * 3, 3).toInt();
        QString engraveName = EngraveManager::getInstance()->getEngraveByIndex(engraveCode);
        int engraveLevel = normalEngraveLevelCode[index].unicode() - '0';

        normalEngraveLevelMap[engraveName] = engraveLevel;
        index++;
    }

    QList<QPair<QString, int>> normalEngraves(normalEngraveLevelMap.keyValueBegin(), normalEngraveLevelMap.keyValueEnd());
    std::sort(normalEngraves.begin(), normalEngraves.end(), [](QPair<QString, int> a, QPair<QString, int> b) {
        return a.second > b.second;
    });

    return normalEngraves;
}

QString SettingCode::generateAbilityCode(const QJsonArray &abilities)
{
    QString abilityCode;
    int value1, value2;

    // Neck
    value1 = static_cast<int>(strToAbility(abilities[0].toString()));
    value2 = static_cast<int>(strToAbility(abilities[1].toString()));
    // 오름차순으로 code 생성
    if (value1 > value2)
        std::swap(value1, value2);
    abilityCode += QString("%1%2").arg(value1).arg(value2);

    // Ear
    value1 = static_cast<int>(strToAbility(abilities[2].toString()));
    value2 = static_cast<int>(strToAbility(abilities[3].toString()));
    if (value1 > value2)
        std::swap(value1, value2);
    abilityCode += QString("%1%2").arg(value1).arg(value2);

    // Ring
    value1 = static_cast<int>(strToAbility(abilities[4].toString()));
    value2 = static_cast<int>(strToAbility(abilities[5].toString()));
    if (value1 > value2)
        std::swap(value1, value2);
    abilityCode += QString("%1%2").arg(value1).arg(value2);

    return "A" + abilityCode;
}

QString SettingCode::generateSetEffectCode(const QJsonArray &setEffects)
{
    QString setEffectCode;
    QList<int> setEffectNumber;

    for (const QJsonValue& setEffect : setEffects)
        setEffectNumber.append(static_cast<int>(strToSetEffect((setEffect.toString()))));
    std::sort(setEffectNumber.begin(), setEffectNumber.end());

    for (const int& setEffect : setEffectNumber)
        setEffectCode += QString("%1").arg(setEffect);

    return "S" + setEffectCode;
}

QString SettingCode::generateEngraveCode(const QJsonArray &engraveNames, const QJsonArray &engraveLevels)
{
    // Mapping engrave name & value
    QMap<QString, int> engraveValue;
    for (int i = 0; i < engraveNames.size(); i++)
        engraveValue[engraveNames[i].toString()] = engraveLevels[i].toInt();

    // 각인명을 고유번호(index)로 변환하여 추가 후 오름차순 정렬
    QList<int> classEngraveNumber;
    QList<int> normalEngraveNumber;
    for (const QJsonValue& engraveName : engraveNames)
    {
        QString engraveNameStr = engraveName.toString();

        if (EngraveManager::getInstance()->isClassEngrave(engraveNameStr))
            classEngraveNumber.append(EngraveManager::getInstance()->indexOf(engraveNameStr));
        else if (EngraveManager::getInstance()->isValidEngrave(engraveNameStr))
            normalEngraveNumber.append(EngraveManager::getInstance()->indexOf(engraveNameStr));
    }
    std::sort(classEngraveNumber.begin(), classEngraveNumber.end());
    std::sort(normalEngraveNumber.begin(), normalEngraveNumber.end());

    // 각인 코드 생성 (직업각인코드 + 직업각인레벨코드 + 일반각인코드 + 일반각인레벨코드)
    QString classEngraveNameCode;
    QString classEngraveLevelCode;
    for (const int& classEngraveIndex : classEngraveNumber)
    {
        classEngraveNameCode += QString("%1").arg(classEngraveIndex, 3, 10, QChar('0'));
        int level = engraveValue[EngraveManager::getInstance()->getEngraveByIndex(classEngraveIndex)];
        if (level > 3)
            level = 3;
        classEngraveLevelCode += QString("%1").arg(level);
    }
    QString normalEngraveNameCode;
    QString normalEngraveLevelCode;
    for (const int& normalEngraveIndex : normalEngraveNumber)
    {
        normalEngraveNameCode += QString("%1").arg(normalEngraveIndex, 3, 10, QChar('0'));
        int level = engraveValue[EngraveManager::getInstance()->getEngraveByIndex(normalEngraveIndex)];
        if (level > 3)
            level = 3;
        normalEngraveLevelCode += QString("%1").arg(level);
    }

    return  "C" + classEngraveNameCode +
            "CL" + classEngraveLevelCode +
            "N" + normalEngraveNameCode +
            "NL" + normalEngraveLevelCode;
}
