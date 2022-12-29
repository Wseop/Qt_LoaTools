#ifndef ENGRAVE_SELECTOR_H
#define ENGRAVE_SELECTOR_H

#include <QWidget>

class QPushButton;

struct Pos
{
    int row;
    int col;
};

namespace Ui {
class EngraveSelector;
}

class EngraveSelector : public QWidget
{
    Q_OBJECT

public:
    EngraveSelector(int buttonIndex);
    ~EngraveSelector();

private:
    void setEngravings();
    void setEngravingButtons();
    QPushButton* createButton(QString text);
    void movePos();

private:
    Ui::EngraveSelector *ui;

    int m_buttonIndex;
    QStringList m_engravings;
    QStringList m_classEngravings;
    QList<QPushButton*> m_btns;

    const int MAX_COL = 6;
    Pos m_pos;
};

#endif // ENGRAVE_SELECTOR_H
