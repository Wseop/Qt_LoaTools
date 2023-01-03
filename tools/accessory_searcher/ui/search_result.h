#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class SearchResult;
}

class SearchResult : public QWidget
{
    Q_OBJECT

public:
    SearchResult();
    ~SearchResult();

    void setItemName(const QString& itemName);
    void setItemGrade(const QString& itemGrade);
    void setItemQuality(int itemQuality);
    void setRemainTime(const QString& endDate);
    void setBuyPrice(int buyPrice);
    void setAbility(const QMap<QString, int>& abilities);
    void setEngraving(const QMap<QString, int>& engravings);

private:
    void setFonts();
    void setAlignments();
    void initConnects();
    QLabel* createLabel(QString text);

private:
    Ui::SearchResult *ui;

    QList<QLabel*> m_labels;
};

#endif // SEARCH_RESULT_H
