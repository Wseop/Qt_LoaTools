#include "MeteorTimer.h"
#include "ui_meteortimer.h"

MeteorTimer::MeteorTimer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteorTimer),
    mTimer(new QTimer()), mCount(MAX_COUNT)
{
    ui->setupUi(this);
    ui->pbHome->setIcon(QIcon(":/resources/Home.ico"));
    ui->pbHome->setIconSize(QSize(30, 30));
    ui->lbCount->setText(QString("%1").arg(mCount));

    initConnect();
}

MeteorTimer::~MeteorTimer()
{
    delete ui;
}

void MeteorTimer::initConnect()
{
    connect(ui->pbHome, SIGNAL(pressed()), this, SLOT(slotHome()));
    connect(ui->pbStart, SIGNAL(pressed()), this, SLOT(slotStartTimer()));
    connect(ui->pbReset, SIGNAL(pressed()), this, SLOT(slotResetTimer()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
}

void MeteorTimer::slotHome()
{
    // Home 화면으로 복귀 시 화면 크기 재조정
    QWidget* parent = static_cast<QWidget*>(this->parent());
    parent->setFixedWidth(800);
    parent->setFixedHeight(600);

    this->close();
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
