#include "ranking_board.h"
#include "ui_ranking_board.h"

RankingBoard* RankingBoard::m_pRankingBoard = nullptr;

RankingBoard::RankingBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingBoard)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 랭킹");
    this->showMaximized();
}

RankingBoard::~RankingBoard()
{
    delete ui;
    destroyInstance();
}

RankingBoard *RankingBoard::getInstance()
{
    if (m_pRankingBoard == nullptr)
        m_pRankingBoard = new RankingBoard();

    return m_pRankingBoard;
}

void RankingBoard::destroyInstance()
{
    if (m_pRankingBoard == nullptr)
        return;

    delete m_pRankingBoard;
    m_pRankingBoard = nullptr;
}
