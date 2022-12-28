#ifndef PENALTY_SELECTOR_H
#define PENALTY_SELECTOR_H

#include <QWidget>

class QPushButton;

namespace Ui {
class PenaltySelector;
}

class PenaltySelector : public QWidget
{
    Q_OBJECT

public:
    PenaltySelector();
    ~PenaltySelector();

private:
    void setPenalties();
    void setPenaltyButtons();
    QPushButton* createButton(QString text);

private:
    Ui::PenaltySelector *ui;

    QStringList m_penalties;
    QList<QPushButton*> m_btns;
};

#endif // PENALTY_SELECTOR_H
