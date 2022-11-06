#ifndef CARD_LABEL_H
#define CARD_LABEL_H

#include <QWidget>

namespace Ui {
class CardLabel;
}

class CardLabel : public QWidget
{
    Q_OBJECT

public:
    explicit CardLabel(QWidget *parent = nullptr, QString cardTitle = "", QString cardDesc = "");
    ~CardLabel();

private:
    Ui::CardLabel *ui;
};

#endif // CARD_LABEL_H
