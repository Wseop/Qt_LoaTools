#ifndef ACCESSORY_SEARCHER_H
#define ACCESSORY_SEARCHER_H

#include <QWidget>

class AuctionOptions;
class EngraveSelector;
class PenaltySelector;
class SearchFilter;
class SearchResult;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;

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
    void sendPostRequest();
    void handleSearchResult(QNetworkReply* pReply);

    QPushButton* createButton(QString text);
    SearchResult* createSearchResult(const QJsonObject& itemObj);

public:
    void setEngraving(int buttonIndex, QString engraving);
    void setPenalty(QString penalty);

    void moveToPickedList(SearchResult* pPicked);
    void deleteFromPickedList(SearchResult* pResult);

    static AccessorySearcher* getInstance();
    static void destroyInstance();

private:
    Ui::AccessorySearcher *ui;

    QList<QPushButton*> m_qualityBtns;
    QList<QPushButton*> m_itemGradeBtns;
    QList<QPushButton*> m_accessoryPartBtns;
    QList<QPushButton*> m_abilityBtns1;
    QList<QPushButton*> m_abilityBtns2;

    QPushButton* m_pSelectedQualityBtn;
    QPushButton* m_pSelectedItemGradeBtn;
    QPushButton* m_pSelectedAccessoryPartBtn;
    QPushButton* m_pSelectedAbilityBtn1;
    QPushButton* m_pSelectedAbilityBtn2;

    QList<EngraveSelector*> m_engraveSelectors;
    PenaltySelector* m_pPenaltySelector;

    SearchFilter* m_pSearchFilter;
    QMap<QString, int> m_engravingToCode;
    QMap<QString, int> m_penaltyToCode;

    QNetworkAccessManager* m_pNetworkManager;
    QList<SearchResult*> m_searchResults;
    QList<SearchResult*> m_pickedResults;

    static AccessorySearcher* m_pInstance;
};

#endif // ACCESSORY_SEARCHER_H
