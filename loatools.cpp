#include "loatools.h"
#include "ui_loatools.h"
#include "tools/meteor_timer/meteor_timer.h"
#include "tools/auction_calc/auction_calc.h"
#include "tools/engrave_simulator/engrave_simulator.h"
#include "tools/setting_adviser/setting_adviser.h"
#include "tools/ranking_board/ranking_board.h"
#include "tools/character_search/character_search.h"
#include "tools/spec_checker/spec_checker.h"
#include "font/font_manager.h"

LoaTools::LoaTools(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoaTools)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));

    initFont();
    initAlignment();
    initConnect();
}

LoaTools::~LoaTools()
{
    delete ui;
}

void LoaTools::initConnect()
{
    connect(ui->pbMeteorTimer, SIGNAL(pressed()), this, SLOT(slotOpenMeteorTimer()));
    connect(ui->pbAuctionCalc, SIGNAL(pressed()), this, SLOT(slotOpenAuctionCalc()));
    connect(ui->pbEngrave, SIGNAL(pressed()), this, SLOT(slotOpenEngraveSimulator()));
    connect(ui->pbCharacterSearch, SIGNAL(pressed()), this, SLOT(slotOpenCharacterSearch()));
    connect(ui->pbSettingAdviser, SIGNAL(pressed()), this, SLOT(slotOpenSettingAdviser()));
    connect(ui->pbRankingBoard, SIGNAL(pressed()), this, SLOT(slotOpenRankingBoard()));
    connect(ui->pbSpecChecker, SIGNAL(pressed()), this, SLOT(slotOpenSpecChecker()));
}

void LoaTools::initFont()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumExtraBold16 = pFontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 16);

    ui->pbAuctionCalc->setFont(fontNanumExtraBold16);
    ui->pbEngrave->setFont(fontNanumExtraBold16);
    ui->pbMeteorTimer->setFont(fontNanumExtraBold16);
    ui->pbCharacterSearch->setFont(fontNanumExtraBold16);
    ui->pbSettingAdviser->setFont(fontNanumExtraBold16);
    ui->pbRankingBoard->setFont(fontNanumExtraBold16);
    ui->pbSpecChecker->setFont(fontNanumExtraBold16);

    ui->groupCharacter->setFont(fontNanumRegular10);
    ui->groupRaid->setFont(fontNanumRegular10);
}

void LoaTools::initAlignment()
{
    ui->hLayoutCharacter1->setAlignment(Qt::AlignLeft);
    ui->hLayoutCharacter2->setAlignment(Qt::AlignLeft);
    ui->hLayoutGroupRaid->setAlignment(Qt::AlignLeft);
}

void LoaTools::slotOpenMeteorTimer()
{
    MeteorTimer::getInstance()->show();
}

void LoaTools::slotOpenAuctionCalc()
{
    AuctionCalc::getInstance()->show();
}

void LoaTools::slotOpenEngraveSimulator()
{
    EngraveSimulator::getInstance()->show();
}

void LoaTools::slotOpenCharacterSearch()
{
    CharacterSearch::getInstance()->show();
}

void LoaTools::slotOpenSettingAdviser()
{
    SettingAdviser::getInstance()->show();
}

void LoaTools::slotOpenRankingBoard()
{
    RankingBoard::getInstance()->show();
}

void LoaTools::slotOpenSpecChecker()
{
    SpecChecker::getInstance()->show();
}
