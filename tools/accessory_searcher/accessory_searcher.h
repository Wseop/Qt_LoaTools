#ifndef ACCESSORY_SEARCHER_H
#define ACCESSORY_SEARCHER_H

#include <QWidget>

class AuctionOptions;
class EngraveSelector;
class PenaltySelector;
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
    void initConnects();
    void setFonts();
    void setAlignments();

    QPushButton* createButton(QString text);

public:
    static AccessorySearcher* getInstance();
    static void destroyInstance();

private:
    Ui::AccessorySearcher *ui;

    QList<QPushButton*> m_qualityBtns;
    QList<QPushButton*> m_itemGradeBtns;
    QList<QPushButton*> m_accessoryPartBtns;
    QList<QPushButton*> m_abilityBtns1;
    QList<QPushButton*> m_abilityBtns2;

    EngraveSelector* m_pEngraveSelector1;
    EngraveSelector* m_pEngraveSelector2;
    PenaltySelector* m_pPenaltySelector;

    static AccessorySearcher* m_pInstance;
};

#endif // ACCESSORY_SEARCHER_H
