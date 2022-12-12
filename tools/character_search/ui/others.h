#ifndef OTHERS_H
#define OTHERS_H

#include "game_data/character/character.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum class MoveType
{
    Server,
    Character,
};

struct GridPos
{
    int row;
    int col;
};

const int MAX_COL = 3;

namespace Ui {
class Others;
}

class Others : public QWidget
{
    Q_OBJECT

public:
    Others(QWidget *parent, const QList<Other>& others);
    ~Others();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void initButtons();
    void movePos(MoveType type);
    QPushButton* createButton(Class cls, int level, double itemLevel, QString name);

private:
    Ui::Others *ui;
    QWidget* m_pParent;
    const QList<Other>& m_others;
    GridPos m_gridPos;
    QMap<QPushButton*, QString> m_btnToName;
    QList<QLabel*> m_serverLabels;
};

#endif // OTHERS_H
