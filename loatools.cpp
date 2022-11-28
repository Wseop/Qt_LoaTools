#include "loatools.h"
#include "ui_loatools.h"
#include "meteor_timer/meteor_timer.h"
#include "auction_calc/auction_calc.h"
#include "engrave_simulator/engrave_simulator.h"
#include "profile/profile.h"
#include "setting_adviser/setting_adviser.h"

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

    ui->groupCharacter->setFont(QFont(fontNanumRegular, 10));
    ui->groupRaid->setFont(QFont(fontNanumRegular, 10));

    ui->hLayoutCharacter1->setAlignment(Qt::AlignLeft);
    ui->hLayoutCharacter2->setAlignment(Qt::AlignLeft);

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
}

void LoaTools::slotOpenMeteorTimer()
{
    if (m_meteorTimer == nullptr)
        m_meteorTimer = new MeteorTimer();
    m_meteorTimer->show();
}

void LoaTools::slotOpenAuctionCalc()
{
    if (m_auctionCalc == nullptr)
        m_auctionCalc = new AuctionCalc();
    m_auctionCalc->show();
}

void LoaTools::slotOpenEngraveSimulator()
{
    if (m_engrave == nullptr)
        m_engrave = new EngraveSimulator();
    m_engrave->show();
}

void LoaTools::slotOpenProfile()
{
    if (m_profile == nullptr)
        m_profile = new Profile();
    m_profile->show();
}

void LoaTools::slotOpenSettingAdviser()
{
    if (m_settingAdviser == nullptr)
        m_settingAdviser = new SettingAdviser();
    m_settingAdviser->show();
}
