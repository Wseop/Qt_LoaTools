#ifndef CARD_WIDGET_H
#define CARD_WIDGET_H

#include <QWidget>
#include <QLabel>

class Card;

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    CardWidget(QWidget* parent, const Card* card);
    ~CardWidget();

private:
    void setFonts();
    void setAlignments();
    void setStyleSheets();
    void addCards();
    QLabel* createName(const QString& name);
    QLabel* createDesc(const QString& desc);

private:
    Ui::CardWidget *ui;
    QWidget* m_pParent;
    const Card* m_pCard;
    QList<QLabel*> m_cardNameLabels;
    QList<QLabel*> m_cardDescLabels;
};

#endif // CARD_WIDGET_H
