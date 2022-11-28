#include "meteor_timer.h"
#include "ui_meteortimer.h"

#include <QFontDatabase>

MeteorTimer::MeteorTimer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeteorTimer),
    mTimer(new QTimer()), mCount(MAX_COUNT)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("메테오 타이머");

    ui->lbCount->setText(QString("%1").arg(mCount));

    QString familyNanumExtraBold = QFontDatabase::applicationFontFamilies(3).at(0);
    QFont fontNanumRegular10 = QFont(QFontDatabase::applicationFontFamilies(1).at(0), 10);
    QFont fontNanumExtraBold20 = QFont(familyNanumExtraBold, 20);
    QFont fontNanumExtraBold48 = QFont(familyNanumExtraBold, 48);

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
