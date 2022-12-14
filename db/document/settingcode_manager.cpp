#include "settingcode_manager.h"
#include "game_data/engrave/engrave_manager.h"

#include <utility>
#include <algorithm>

#include <QMap>
#include <QDebug>

QString SettingCodeManager::generateSettingCode(const QList<Ability>& abilities, const QList<SetEffect>& setEffects, const QList<QPair<QString, int>>& engraves)
{
    return generateAbilityCode(abilities) +
           generateSetEffectCode(setEffects) +
            generateEngraveCode(engraves);
}

QList<Ability> SettingCodeManager::parseAbilities(QString settingCode)
{
    int startIndex = settingCode.indexOf("A") + 1;
    int endIndex = settingCode.indexOf("S");
    QString abilityCode = settingCode.sliced(startIndex, endIndex - startIndex);
    QList<Ability> abilities;

    int codeSize = abilityCode.size();
    if (codeSize != 6)
    {
        qDebug() << Q_FUNC_INFO << "Invalid Ability Code";
        return abilities;
    }

    for (int i = 0; i < codeSize; i++)
    {
        abilities.append(static_cast<Ability>(abilityCode[i].unicode() - '0'));
    }

    return abilities;
}

QList<SetEffect> SettingCodeManager::parseSetEffects(QString settingCode)
{
    int startIndex = settingCode.indexOf("S") + 1;
    int endIndex = settingCode.indexOf("C");
    QString setEffectCode = settingCode.sliced(startIndex, endIndex - startIndex);
    QList<SetEffect> setEffects;

    int codeSize = setEffectCode.size();
    if (codeSize != 6)
    {
        qDebug() << Q_FUNC_INFO << "Invalid SetEffect Code";
        return setEffects;
    }

    for (int i = 0; i < codeSize; i++)
    {
        setEffects.append(static_cast<SetEffect>(setEffectCode[i].unicode() - '0'));
    }

    return setEffects;
}

QList<QPair<QString, int>> SettingCodeManager::parseClassEngraves(QString settingCode)
{
    EngraveManager* pEngraveManager = EngraveManager::getInstance();

    int startIndex = settingCode.indexOf("C") + 1;
    int endIndex = settingCode.indexOf("CL");
    QString engraveNameCode = settingCode.sliced(startIndex, endIndex - startIndex);

    startIndex = settingCode.indexOf("CL") + 2;
    endIndex = settingCode.indexOf("N");
    QString engraveLevelCode = settingCode.sliced(startIndex, endIndex - startIndex);

    QList<QPair<QString, int>> classEngraves;
    // level 3 -> 2 -> 1????????? ??????
    for (int level = 3; level > 0; level--)
    {
        for (int i = 0; i < engraveLevelCode.size(); i++)
        {
            int engraveLevel = engraveLevelCode[i].unicode() - '0';
            if (engraveLevel != level)
                continue;

            QString engraveName = pEngraveManager->getEngraveByIndex(engraveNameCode.sliced(i * 3, 3).toInt());
            classEngraves.append({engraveName, engraveLevel});
        }
    }

    return classEngraves;
}

QList<QPair<QString, int>> SettingCodeManager::parseNormalEngraves(QString settingCode)
{
    EngraveManager* pEngraveManager = EngraveManager::getInstance();

    int startIndex = settingCode.indexOf("N") + 1;
    int endIndex = settingCode.indexOf("NL");
    QString engraveNameCode = settingCode.sliced(startIndex, endIndex - startIndex);

    startIndex = settingCode.indexOf("NL") + 2;
    endIndex = settingCode.size();
    QString engraveLevelCode = settingCode.sliced(startIndex, endIndex - startIndex);

    QList<QPair<QString, int>> normalEngraves;
    for (int level = 3; level > 0; level--)
    {
        for (int i = 0; i < engraveLevelCode.size(); i++)
        {
            int engraveLevel = engraveLevelCode[i].unicode() - '0';
            if (engraveLevel != level)
                continue;

            QString engraveName = pEngraveManager->getEngraveByIndex(engraveNameCode.sliced(i * 3, 3).toInt());
            normalEngraves.append({engraveName, engraveLevel});
        }
    }

    return normalEngraves;
}

QString SettingCodeManager::generateAbilityCode(const QList<Ability>& abilities)
{
    QString abilityCode;

    // ????????? ?????? [0], [1]
    // ????????? ?????? [2], [3]
    // ?????? ?????? [4], [5]
    for (int i = 0; i < 6; i += 2)
    {
        int value1 = static_cast<int>(abilities[i]);
        int value2 = static_cast<int>(abilities[i + 1]);
        // ????????? ???????????? ??????
        if (value1 > value2)
            std::swap(value1, value2);
        abilityCode += QString::number(value1);
        abilityCode += QString::number(value2);
    }

    return "A" + abilityCode;
}

QString SettingCodeManager::generateSetEffectCode(const QList<SetEffect>& setEffects)
{
    QString setEffectCode;

    QList<int> setEffectNumbers;
    for (const SetEffect& setEffect : setEffects)
    {
        setEffectNumbers.append(static_cast<int>(setEffect));
    }
    // ????????? ?????? ?????? ????????? ?????? ??????
    std::sort(setEffectNumbers.begin(), setEffectNumbers.end());
    for (const int& setEffectNumber : setEffectNumbers)
    {
        setEffectCode += QString::number(setEffectNumber);
    }

    return "S" + setEffectCode;
}

QString SettingCodeManager::generateEngraveCode(const QList<QPair<QString, int>>& engraves)
{
    EngraveManager* pEngraveManager = EngraveManager::getInstance();

    // ????????? -> index??? ???????????? ????????? ???????????? ??????
    QList<int> classEngraveIndices;
    QList<int> normalEngraveIndices;
    for (const auto& engrave : engraves)
    {
        const QString& engraveName = engrave.first;

        if (pEngraveManager->isClassEngrave(engraveName))
            classEngraveIndices.append(pEngraveManager->indexOf(engraveName));
        else if (pEngraveManager->isValidEngrave(engraveName))
            normalEngraveIndices.append(pEngraveManager->indexOf(engraveName));
    }
    std::sort(classEngraveIndices.begin(), classEngraveIndices.end());
    std::sort(normalEngraveIndices.begin(), normalEngraveIndices.end());

    // {?????????, ????????????} map ??????
    QMap<QString, int> engraveToLevel;
    for (const auto& engrave : engraves)
    {
        engraveToLevel[engrave.first] = engrave.second;
    }

    // ?????? ?????? ?????? ?????? ?????? (?????? indices + ?????? level ??????)
    QString classEngraveIndexCode;
    QString classEngraveLevelCode;
    QString normalEngraveIndexCode;
    QString normalEngraveLevelCode;
    // ?????? ??????
    for (const int& index : classEngraveIndices)
    {
        // ?????? index -> 3????????? ????????? ???????????? ??????
        classEngraveIndexCode += QString("%1").arg(index, 3, 10, QChar('0'));

        int level = engraveToLevel[pEngraveManager->getEngraveByIndex(index)];
        if (level > 3)
            level = 3;
        classEngraveLevelCode += QString::number(level);
    }
    // ?????? ??????
    for (const int& index : normalEngraveIndices)
    {
        normalEngraveIndexCode += QString("%1").arg(index, 3, 10, QChar('0'));

        int level = engraveToLevel[pEngraveManager->getEngraveByIndex(index)];
        if (level > 3)
            level = 3;
        normalEngraveLevelCode += QString::number(level);
    }

    return "C" + classEngraveIndexCode +
           "CL" + classEngraveLevelCode +
           "N" + normalEngraveIndexCode +
           "NL" + normalEngraveLevelCode;
}
