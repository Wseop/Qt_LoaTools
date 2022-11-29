#include "loatools.h"
#include "ui_loatools.h"
#include "meteor_timer/meteor_timer.h"
#include "auction_calc/auction_calc.h"
#include "engrave_simulator/engrave_simulator.h"
#include "profile/profile.h"
#include "setting_adviser/setting_adviser.h"
#include "ranking_board/ranking_board.h"

#include <QFontDatabase>

LoaTools::LoaTools(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoaTools)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));

    QString fontNanumExtraBold = QFontDatabase::applicationFontFamilies(3).at(0);
    QString fontNanumRegular = QFontDatabase::applicationFontFamilies(1).at(0);

    ui->pbAuctionCalc->setFont(QFont(fontNanumExtraBold, 16));
    ui->pbEngrave->setFont(QFont(fontNanumExtraBold, 16));
    ui->pbMeteorTimer->setFont(QFont(fontNanumExtraBold, 16));
    ui->pbProfile->setFont(QFont(fontNanumExtraBold, 16));
    ui->pbSettingAdviser->setFont(QFont(fontNanumExtraBold, 16));
    ui->pbRankingBoard->setFont(QFont(fontNanumExtraBold, 16));

    ui->groupCharacter->setFont(QFont(fontNanumRegular, 10));
    ui->groupRaid->setFont(QFont(fontNanumRegular, 10));

    ui->hLayoutCharacter1->setAlignment(Qt::AlignLeft);
    ui->hLayoutCharacter2->setAlignment(Qt::AlignLeft);
    ui->hLayoutGroupRaid->setAlignment(Qt::AlignLeft);

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
    connect(ui->pbProfile, SIGNAL(pressed()), this, SLOT(slotOpenProfile()));
    connect(ui->pbSettingAdviser, SIGNAL(pressed()), this, SLOT(slotOpenSettingAdviser()));
    connect(ui->pbRankingBoard, SIGNAL(pressed()), this, SLOT(slotOpenRankingBoard()));
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

void LoaTools::slotOpenProfile()
{
    Profile::getInstance()->show();
}

void LoaTools::slotOpenSettingAdviser()
{
    SettingAdviser::getInstance()->show();
}

void LoaTools::slotOpenRankingBoard()
{
    RankingBoard::getInstance()->show();
}
