#include "loatools.h"
#include "ui_loatools.h"
#include "MeteorTimer/MeteorTimer.h"
#include "AuctionCalc/AuctionCalc.h"
#include "EngraveSimulator/EngraveSimulator.h"
#include "Profile/Profile.h"

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
    this->setFixedSize(400, 300);
}

void LoaTools::slotOpenAuctionCalc()
{
    if (mAuctionCalc == nullptr)
        mAuctionCalc = new AuctionCalc();
    mAuctionCalc->show();
    this->setFixedSize(411, 300);
}

void LoaTools::slotOpenEngraveSimulator()
{
    if (mEngrave == nullptr)
        mEngrave = new EngraveSimulator();
    mEngrave->show();
    this->setFixedSize(865, 570);
}

void LoaTools::slotOpenProfile()
{
    if (mProfile == nullptr)
        mProfile = new Profile();
    mProfile->show();
}
