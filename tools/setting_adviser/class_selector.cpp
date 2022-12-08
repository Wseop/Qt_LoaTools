#include "class_selector.h"
#include "ui_class_selector.h"
#include "tools/setting_adviser/setting_adviser.h"
#include "tools/ranking_board/ranking_board.h"
#include "font/font_manager.h"

#include <QCloseEvent>

ClassSelector::ClassSelector(QWidget *parent, ParentClass parentClass) :
    ui(new Ui::ClassSelector),
    m_pParent(parent),
    m_parentClass(parentClass)
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

    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);
    QFont fontNanumExtraBold12 = fontManager->getFont(FontFamily::NanumSquareNeoExtraBold, 12);
    ui->pbDestroyer->setFont(fontNanumBold12);
    ui->pbWarlord->setFont(fontNanumBold12);
    ui->pbBerserker->setFont(fontNanumBold12);
    ui->pbHolyknight->setFont(fontNanumBold12);
    ui->pbStriker->setFont(fontNanumBold12);
    ui->pbBattlemaster->setFont(fontNanumBold12);
    ui->pbInfighter->setFont(fontNanumBold12);
    ui->pbSoulmaster->setFont(fontNanumBold12);
    ui->pbLancemaster->setFont(fontNanumBold12);
    ui->pbDevilHunter->setFont(fontNanumBold12);
    ui->pbBlaster->setFont(fontNanumBold12);
    ui->pbHawkeye->setFont(fontNanumBold12);
    ui->pbScouter->setFont(fontNanumBold12);
    ui->pbGunslinger->setFont(fontNanumBold12);
    ui->pbBard->setFont(fontNanumBold12);
    ui->pbSummoner->setFont(fontNanumBold12);
    ui->pbArcana->setFont(fontNanumBold12);
    ui->pbSorceress->setFont(fontNanumBold12);
    ui->pbBlade->setFont(fontNanumBold12);
    ui->pbDemonic->setFont(fontNanumBold12);
    ui->pbReaper->setFont(fontNanumBold12);
    ui->pbArtist->setFont(fontNanumBold12);
    ui->pbAeromancer->setFont(fontNanumBold12);
}

void ClassSelector::initConnect()
{   
    connect(ui->pbDestroyer, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::디스트로이어);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::디스트로이어);
    });
    connect(ui->pbWarlord, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::워로드);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::워로드);
    });
    connect(ui->pbBerserker, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::버서커);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::버서커);
    });
    connect(ui->pbHolyknight, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::홀리나이트);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::홀리나이트);
    });
    connect(ui->pbStriker, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::스트라이커);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::스트라이커);
    });
    connect(ui->pbBattlemaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::배틀마스터);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::배틀마스터);
    });
    connect(ui->pbInfighter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::인파이터);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::인파이터);
    });
    connect(ui->pbSoulmaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::기공사);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::기공사);
    });
    connect(ui->pbLancemaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::창술사);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::창술사);
    });
    connect(ui->pbDevilHunter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::데빌헌터);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::데빌헌터);
    });
    connect(ui->pbBlaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::블래스터);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::블래스터);
    });
    connect(ui->pbHawkeye, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::호크아이);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::호크아이);
    });
    connect(ui->pbScouter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::스카우터);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::스카우터);
    });
    connect(ui->pbGunslinger, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::건슬링어);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::건슬링어);
    });
    connect(ui->pbBard, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::바드);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::바드);
    });
    connect(ui->pbSummoner, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::서머너);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::서머너);
    });
    connect(ui->pbArcana, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::아르카나);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::아르카나);
    });
    connect(ui->pbSorceress, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::소서리스);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::소서리스);
    });
    connect(ui->pbBlade, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::블레이드);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::블레이드);
    });
    connect(ui->pbDemonic, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::데모닉);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::데모닉);
    });
    connect(ui->pbReaper, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::리퍼);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::리퍼);
    });
    connect(ui->pbArtist, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::도화가);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::도화가);
    });
    connect(ui->pbAeromancer, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::기상술사);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::기상술사);
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
