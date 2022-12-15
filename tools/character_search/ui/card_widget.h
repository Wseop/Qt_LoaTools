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
    CardWidget(QWidget* pParent, const Card* pCard);
    ~CardWidget();

private:
    void setLabels();
    QLabel* createNameLabel(const QString& name);
    QLabel* createDescLabel(const QString& desc);
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::CardWidget *ui;
    QWidget* m_pParent;
    const Card* m_pCard;
    QList<QLabel*> m_cardNameLabels;
    QList<QLabel*> m_cardDescLabels;
};

#endif // CARD_WIDGET_H
