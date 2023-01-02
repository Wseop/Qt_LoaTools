#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <QWidget>

namespace Ui {
class SearchResult;
}

class SearchResult : public QWidget
{
    Q_OBJECT

public:
    SearchResult();
    ~SearchResult();

    void setItemName(QString itemName);
    void setItemGrade(QString itemGrade);
    void setItemQuality(int itemQuality);
    void setRemainTime(QString endDate);
    void setBuyPrice(int buyPrice);

private:
    void setFonts();
    void initConnects();

private:
    Ui::SearchResult *ui;
};

#endif // SEARCH_RESULT_H
