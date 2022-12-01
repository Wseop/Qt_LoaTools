#include "meteor_timer.h"
#include "ui_meteortimer.h"
#include "font/font_manager.h"

MeteorTimer* MeteorTimer::m_pMeteorTimer = nullptr;

MeteorTimer::MeteorTimer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteorTimer),
    mTimer(new QTimer()), mCount(MAX_COUNT)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("메테오 타이머");

    ui->lbCount->setText(QString("%1").arg(mCount));

    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumExtraBold20 = fontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 20);
    QFont fontNanumExtraBold48 = fontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 48);

    ui->groupBox->setFont(fontNanumRegular10);
    ui->pbReset->setFont(fontNanumExtraBold20);
    ui->pbStart->setFont(fontNanumExtraBold20);
    ui->lbCount->setFont(fontNanumExtraBold48);

    initConnect();
}

MeteorTimer::~MeteorTimer()
{
    delete mTimer;
    delete ui;
    destroyInstance();
}

MeteorTimer *MeteorTimer::getInstance()
{
    if (m_pMeteorTimer == nullptr)
        m_pMeteorTimer = new MeteorTimer();

    return m_pMeteorTimer;
}

void MeteorTimer::destroyInstance()
{
    if (m_pMeteorTimer == nullptr)
        return;

    delete m_pMeteorTimer;
    m_pMeteorTimer = nullptr;
}

void MeteorTimer::initConnect()
{
    connect(ui->pbStart, SIGNAL(pressed()), this, SLOT(slotStartTimer()));
    connect(ui->pbReset, SIGNAL(pressed()), this, SLOT(slotResetTimer()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
}

void MeteorTimer::slotStartTimer()
{
    mTimer->start(1000);
    ui->pbStart->setEnabled(false);
}

void MeteorTimer::slotResetTimer()
{
    mTimer->stop();
    mCount = MAX_COUNT;
    ui->lbCount->setText(QString("%1").arg(mCount));
    ui->pbStart->setEnabled(true);
}

void MeteorTimer::slotUpdateTimer()
{
    // count 갱신
    mCount--;
    ui->lbCount->setText(QString("%1").arg(mCount));

    // count 만료 시 reset
    if (mCount <= 0)
    {
        slotResetTimer();
    }
}
