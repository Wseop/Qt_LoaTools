#include "loatools.h"
#include "ui_loatools.h"
#include "MeteorTimer/MeteorTimer.h"
#include "AuctionCalc/AuctionCalc.h"

LoaTools::LoaTools(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoaTools)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));

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
}

void LoaTools::slotOpenMeteorTimer()
{
    mMeteorTimer = new MeteorTimer(this);
    mMeteorTimer->show();
    this->setFixedWidth(400);
    this->setFixedHeight(300);
}

void LoaTools::slotOpenAuctionCalc()
{
    mAuctionCalc = new AuctionCalc(this);
    mAuctionCalc->show();
    this->setFixedWidth(400);
    this->setFixedHeight(300);
}
