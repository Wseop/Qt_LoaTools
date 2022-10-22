#include "loatools.h"
#include "ui_loatools.h"
#include "MeteorTimer/meteor_timer.h"
#include "AuctionCalc/auction_calc.h"
#include "EngraveSimulator/engrave_simulator.h"
#include "Profile/profile.h"

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
    connect(ui->pbEngrave, SIGNAL(pressed()), this, SLOT(slotOpenEngraveSimulator()));
    connect(ui->pbProfile, SIGNAL(pressed()), this, SLOT(slotOpenProfile()));
}

void LoaTools::slotOpenMeteorTimer()
{
    if (mMeteorTimer == nullptr)
        mMeteorTimer = new MeteorTimer();
    mMeteorTimer->show();
}

void LoaTools::slotOpenAuctionCalc()
{
    if (mAuctionCalc == nullptr)
        mAuctionCalc = new AuctionCalc();
    mAuctionCalc->show();
}

void LoaTools::slotOpenEngraveSimulator()
{
    if (mEngrave == nullptr)
        mEngrave = new EngraveSimulator();
    mEngrave->show();
}

void LoaTools::slotOpenProfile()
{
    if (mProfile == nullptr)
        mProfile = new Profile();
    mProfile->show();
}
