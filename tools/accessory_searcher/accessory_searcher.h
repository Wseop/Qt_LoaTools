#ifndef ACCESSORY_SEARCHER_H
#define ACCESSORY_SEARCHER_H

#include <QWidget>

class AuctionOptions;
class EngraveSelector;
class PenaltySelector;
class SearchFilter;
class QPushButton;

namespace Ui {
class AccessorySearcher;
}

class AccessorySearcher : public QWidget
{
    Q_OBJECT

private:
    AccessorySearcher();
    ~AccessorySearcher();

    void initQualityBtns();
    void initItemGradeBtns();
    void initAccessoryPartBtns();
    void initAbilityBtns();
    void initLineEditValidators();
    void initConnects();
    void setFonts();
    void setAlignments();
    void updateSearchFilter();
    void initFilter();

    QPushButton* createButton(QString text);

public:
    void setEngraving(int buttonIndex, QString engraving);
    void setPenalty(QString penalty);

    static AccessorySearcher* getInstance();
    static void destroyInstance();

private:
    Ui::AccessorySearcher *ui;

    QList<QPushButton*> m_qualityBtns;
    QList<QPushButton*> m_itemGradeBtns;
    QList<QPushButton*> m_accessoryPartBtns;
    QList<QPushButton*> m_abilityBtns1;
    QList<QPushButton*> m_abilityBtns2;

    QList<EngraveSelector*> m_engraveSelectors;
    PenaltySelector* m_pPenaltySelector;

    SearchFilter* m_pSearchFilter;
    QMap<QString, int> m_engravingToCode;
    QMap<QString, int> m_penaltyToCode;

    static AccessorySearcher* m_pInstance;
};

#endif // ACCESSORY_SEARCHER_H
