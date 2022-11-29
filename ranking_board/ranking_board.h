#ifndef RANKING_BOARD_H
#define RANKING_BOARD_H

#include <QWidget>

namespace Ui {
class RankingBoard;
}

class RankingBoard : public QWidget
{
    Q_OBJECT

private:
    explicit RankingBoard(QWidget *parent = nullptr);
    ~RankingBoard();

public:
    static RankingBoard* getInstance();
    static void destroyInstance();

private:
    Ui::RankingBoard *ui;

    static RankingBoard* m_pRankingBoard;
};

#endif // RANKING_BOARD_H
