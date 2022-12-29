#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QObject>
#include <QJsonObject>

class SearchFilter : public QObject
{
    Q_OBJECT

public:
    SearchFilter();

    void setQuality(int quality);
    void setItemGrade(QString itemGrade);
    void setCategory(int categoryCode);
    void setAbilityCode1(int abilityCode);
    void setAbilityMinValue1(int minValue = -1);
    void setAbilityMaxValue1(int maxValue = -1);
    void setAbilityCode2(int abilityCode);
    void setAbilityMinValue2(int minValue = -1);
    void setAbilityMaxValue2(int maxValue = -1);
    void setEngraving1(int engravingCode);
    void setEngravingMinValue1(int minValue = -1);
    void setEngravingMaxValue1(int maxValue = -1);
    void setEngraving2(int engravingCode);
    void setEngravingMinValue2(int minValue = -1);
    void setEngravingMaxValue2(int maxValue = -1);
    void setPenalty(int penaltyCode);
    void setPenaltyMinValue(int minValue = -1);
    void setPenaltyMaxValue(int maxValue = -1);
    void setPageNo(int pageNo);

    QJsonObject getFilterObj();

private:
    QJsonObject buildEtcOption(int firstOption, int secondOption, int minValue, int maxValue);

private:
    int m_quality;
    QString m_itemGrade;
    int m_categoryCode;
    int m_abilityCode1;
    int m_abilityMinValue1;
    int m_abilityMaxValue1;
    int m_abilityCode2;
    int m_abilityMinValue2;
    int m_abilityMaxValue2;
    int m_engravingCode1;
    int m_engravingMinValue1;
    int m_engravingMaxValue1;
    int m_engravingCode2;
    int m_engravingMinValue2;
    int m_engravingMaxValue2;
    int m_penaltyCode;
    int m_penaltyMinValue;
    int m_penaltyMaxValue;
    int m_pageNo;
};

#endif // SEARCHFILTER_H
