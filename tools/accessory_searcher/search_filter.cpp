#include "search_filter.h"

#include <QJsonArray>

SearchFilter::SearchFilter() :
    m_quality(-1),
    m_itemGrade(""),
    m_categoryCode(200000),
    m_abilityCode1(-1), m_abilityMinValue1(-1), m_abilityMaxValue1(-1),
    m_abilityCode2(-1), m_abilityMinValue2(-1), m_abilityMaxValue2(-1),
    m_engravingCode1(-1), m_engravingMinValue1(-1), m_engravingMaxValue1(-1),
    m_engravingCode2(-1), m_engravingMinValue2(-1), m_engravingMaxValue2(-1),
    m_penaltyCode(-1), m_penaltyMinValue(-1), m_penaltyMaxValue(-1),
    m_pageNo(1)
{

}

void SearchFilter::setQuality(int quality)
{
    m_quality = quality;
}

void SearchFilter::setItemGrade(QString itemGrade)
{
    m_itemGrade = itemGrade;
}

void SearchFilter::setCategory(int categoryCode)
{
    m_categoryCode = categoryCode;
}

void SearchFilter::setAbilityCode1(int abilityCode)
{
    m_abilityCode1 = abilityCode;
}

void SearchFilter::setAbilityMinValue1(int minValue)
{
    m_abilityMinValue1 = minValue;
}

void SearchFilter::setAbilityMaxValue1(int maxValue)
{
    m_abilityMaxValue1 = maxValue;
}

void SearchFilter::setAbilityCode2(int abilityCode)
{
    m_abilityCode2 = abilityCode;
}

void SearchFilter::setAbilityMinValue2(int minValue)
{
    m_abilityMinValue2 = minValue;
}

void SearchFilter::setAbilityMaxValue2(int maxValue)
{
    m_abilityMaxValue2 = maxValue;
}

void SearchFilter::setEngraving1(int engravingCode)
{
    m_engravingCode1 = engravingCode;
}

void SearchFilter::setEngravingMinValue1(int minValue)
{
    m_engravingMinValue1 = minValue;
}

void SearchFilter::setEngravingMaxValue1(int maxValue)
{
    m_engravingMaxValue1 = maxValue;
}

void SearchFilter::setEngraving2(int engravingCode)
{
    m_engravingCode2 = engravingCode;
}

void SearchFilter::setEngravingMinValue2(int minValue)
{
    m_engravingMinValue2 = minValue;
}

void SearchFilter::setEngravingMaxValue2(int maxValue)
{
    m_engravingMaxValue2 = maxValue;
}

void SearchFilter::setPenalty(int penaltyCode)
{
    m_penaltyCode = penaltyCode;
}

void SearchFilter::setPenaltyMinValue(int minValue)
{
    m_penaltyMinValue = minValue;
}

void SearchFilter::setPenaltyMaxValue(int maxValue)
{
    m_penaltyMaxValue = maxValue;
}

void SearchFilter::setPageNo(int pageNo)
{
    m_pageNo = pageNo;
}

QJsonObject SearchFilter::getFilterObj()
{
    QJsonObject filterObj;

    // ItemGradeQuality
    if (m_quality != -1)
        filterObj.insert("ItemGradeQuality", m_quality);

    // EtcOptions
    QJsonArray etcOptions;
    if (m_abilityCode1 != -1)
    {
        QJsonObject etcOption = buildEtcOption(2, m_abilityCode1, m_abilityMinValue1, m_abilityMaxValue1);
        etcOptions.append(etcOption);
    }
    if (m_abilityCode2 != -1)
    {
        QJsonObject etcOption = buildEtcOption(2, m_abilityCode2, m_abilityMinValue2, m_abilityMaxValue2);
        etcOptions.append(etcOption);
    }
    if (m_engravingCode1 != -1)
    {
        QJsonObject etcOption = buildEtcOption(3, m_engravingCode1, m_engravingMinValue1, m_engravingMaxValue1);
        etcOptions.append(etcOption);
    }
    if (m_engravingCode2 != -1)
    {
        QJsonObject etcOption = buildEtcOption(3, m_engravingCode2, m_engravingMinValue2, m_engravingMaxValue2);
        etcOptions.append(etcOption);
    }
    if (m_penaltyCode != -1)
    {
        QJsonObject etcOption = buildEtcOption(6, m_penaltyCode, m_penaltyMinValue, m_penaltyMaxValue);
        etcOptions.append(etcOption);
    }
    if (etcOptions.size() != 0)
    {
        filterObj.insert("EtcOptions", etcOptions);
    }

    // Sort
    filterObj.insert("Sort", "BUY_PRICE");
    // CategoryCode
    filterObj.insert("CategoryCode", m_categoryCode);
    // ItemTier
    filterObj.insert("ItemTier", 3);
    // ItemGrade
    if (m_itemGrade != "")
        filterObj.insert("ItemGrade", m_itemGrade);
    // PageNo
    filterObj.insert("PageNo", m_pageNo);
    // SortCondition
    filterObj.insert("SortCondition", "ASC");

    return filterObj;
}

QJsonObject SearchFilter::buildEtcOption(int firstOption, int secondOption, int minValue, int maxValue)
{
    QJsonObject etcOption;

    etcOption.insert("FirstOption", firstOption);
    etcOption.insert("SecondOption", secondOption);
    if (minValue != -1)
        etcOption.insert("MinValue", minValue);
    if (maxValue != -1)
        etcOption.insert("MaxValue", maxValue);

    return etcOption;
}
