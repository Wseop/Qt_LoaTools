#include "class_selector.h"
#include "ui_class_selector.h"
#include "setting_adviser/setting_adviser.h"
#include "ranking_board/ranking_board.h"

#include <QCloseEvent>
#include <QFontDatabase>

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

    QFont fontNanumBold12 = QFont(QFontDatabase::applicationFontFamilies(2).at(0), 12);
    QFont fontNanumExtraBold12 = QFont(QFontDatabase::applicationFontFamilies(3).at(0), 12);
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
            SettingAdviser::getInstance()->setSelectedClass(Class::Destroyer);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Destroyer);
    });
    connect(ui->pbWarlord, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Warlord);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Warlord);
    });
    connect(ui->pbBerserker, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Berserker);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Berserker);
    });
    connect(ui->pbHolyknight, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Holyknight);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Holyknight);
    });
    connect(ui->pbStriker, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Striker);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Striker);
    });
    connect(ui->pbBattlemaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Battlemaster);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Battlemaster);
    });
    connect(ui->pbInfighter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Infighter);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Infighter);
    });
    connect(ui->pbSoulmaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Soulmaster);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Soulmaster);
    });
    connect(ui->pbLancemaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Lancemaster);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Lancemaster);
    });
    connect(ui->pbDevilHunter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::DevilHunter);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::DevilHunter);
    });
    connect(ui->pbBlaster, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Blaster);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Blaster);
    });
    connect(ui->pbHawkeye, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Hawkeye);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Hawkeye);
    });
    connect(ui->pbScouter, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Scouter);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Scouter);
    });
    connect(ui->pbGunslinger, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Gunslinger);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Gunslinger);
    });
    connect(ui->pbBard, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Bard);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Bard);
    });
    connect(ui->pbSummoner, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Summoner);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Summoner);
    });
    connect(ui->pbArcana, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Arcana);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Arcana);
    });
    connect(ui->pbSorceress, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Sorceress);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Sorceress);
    });
    connect(ui->pbBlade, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Blade);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Blade);
    });
    connect(ui->pbDemonic, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Demonic);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Demonic);
    });
    connect(ui->pbReaper, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Reaper);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Reaper);
    });
    connect(ui->pbArtist, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Artist);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Artist);
    });
    connect(ui->pbAeromancer, &QPushButton::pressed, [&]() {
        if (m_parentClass == ParentClass::SettingAdviser)
            SettingAdviser::getInstance()->setSelectedClass(Class::Aeromancer);
        else if (m_parentClass == ParentClass::RankingBoard)
            RankingBoard::getInstance()->setSelectedClass(Class::Aeromancer);
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
