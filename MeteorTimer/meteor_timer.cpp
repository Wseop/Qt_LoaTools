#include "meteor_timer.h"
#include "ui_meteortimer.h"

MeteorTimer::MeteorTimer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteorTimer),
    mTimer(new QTimer()), mCount(MAX_COUNT)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("메테오 타이머");

    ui->lbCount->setText(QString("%1").arg(mCount));

    initConnect();
}

MeteorTimer::~MeteorTimer()
{
    delete ui;
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
