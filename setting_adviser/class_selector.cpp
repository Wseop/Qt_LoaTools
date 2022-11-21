#include "class_selector.h"
#include "ui_class_selector.h"
#include "setting_adviser/setting_adviser.h"

#include <QCloseEvent>

ClassSelector::ClassSelector(QWidget *parent, QWidget* ret) :
    QWidget(parent),
    ui(new Ui::ClassSelector),
    m_pParent(ret)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("직업 선택");

    initStyleSheet();
    initConnect();
}

ClassSelector::~ClassSelector()
{
    delete ui;
}

void ClassSelector::closeEvent(QCloseEvent *event)
{
    m_pParent->setEnabled(true);
    event->accept();
}

void ClassSelector::initStyleSheet()
{
    QString styleSheet = "QLabel { color: #B178FF; background-color: #444A5B }";

    ui->lbWarrior->setStyleSheet(styleSheet);
    ui->lbFighterMale->setStyleSheet(styleSheet);
    ui->lbFighterFemale->setStyleSheet(styleSheet);
    ui->lbHunterMale->setStyleSheet(styleSheet);
    ui->lbHunterFemale->setStyleSheet(styleSheet);
    ui->lbMagician->setStyleSheet(styleSheet);
    ui->lbAssassin->setStyleSheet(styleSheet);
    ui->lbSpecialist->setStyleSheet(styleSheet);
}

void ClassSelector::initConnect()
{
    connect(ui->pbDestroyer, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("디스트로이어");
    });
    connect(ui->pbWarlord, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("워로드");
    });
    connect(ui->pbBerserker, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("버서커");
    });
    connect(ui->pbHolyknight, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("홀리나이트");
    });
    connect(ui->pbStriker, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("스트라이커");
    });
    connect(ui->pbBattlemaster, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("배틀마스터");
    });
    connect(ui->pbInfighter, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("인파이터");
    });
    connect(ui->pbSoulmaster, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("기공사");
    });
    connect(ui->pbLancemaster, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("창술사");
    });
    connect(ui->pbDevilHunter, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("데빌헌터");
    });
    connect(ui->pbBlaster, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("블래스터");
    });
    connect(ui->pbHawkeye, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("호크아이");
    });
    connect(ui->pbScouter, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("스카우터");
    });
    connect(ui->pbGunslinger, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("건슬링어");
    });
    connect(ui->pbBard, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("바드");
    });
    connect(ui->pbSummoner, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("서머너");
    });
    connect(ui->pbArcana, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("아르카나");
    });
    connect(ui->pbSorceress, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("소서리스");
    });
    connect(ui->pbBlade, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("블레이드");
    });
    connect(ui->pbDemonic, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("데모닉");
    });
    connect(ui->pbReaper, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("리퍼");
    });
    connect(ui->pbArtist, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("도화가");
    });
    connect(ui->pbAeromancer, &QPushButton::pressed, [&]() {
        SettingAdviser* pSettingAdviser = static_cast<SettingAdviser*>(m_pParent);
        pSettingAdviser->setSelectedClass("기상술사");
    });
    connect(ui->pbDestroyer, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbWarlord, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbBerserker, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbHolyknight, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbStriker, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbBattlemaster, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbInfighter, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbSoulmaster, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbLancemaster, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbDevilHunter, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbBlaster, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbHawkeye, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbScouter, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbGunslinger, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbBard, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbSummoner, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbArcana, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbSorceress, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbBlade, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbDemonic, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbReaper, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbArtist, SIGNAL(pressed()), this, SLOT(close()));
    connect(ui->pbAeromancer, SIGNAL(pressed()), this, SLOT(close()));
}
