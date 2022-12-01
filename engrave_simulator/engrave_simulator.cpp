#include "engrave_simulator.h"
#include "ui_engravesimulator.h"
#include "font/font_manager.h"

#include <QCompleter>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

EngraveSimulator* EngraveSimulator::m_pEngraveSimulator = nullptr;

EngraveSimulator::EngraveSimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EngraveSimulator),
    mEngraveLayout(new QHBoxLayout())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("각인 계산기");

    initMap();
    initUI();
    initConnect();
}

EngraveSimulator::~EngraveSimulator()
{
    delete ui;
    destroyInstance();
}

EngraveSimulator *EngraveSimulator::getInstance()
{
    if (m_pEngraveSimulator == nullptr)
        m_pEngraveSimulator = new EngraveSimulator();

    return m_pEngraveSimulator;
}

void EngraveSimulator::destroyInstance()
{
    if (m_pEngraveSimulator == nullptr)
        return;

    delete m_pEngraveSimulator;
    m_pEngraveSimulator = nullptr;
}

void EngraveSimulator::initMap()
{
    // LineEdit Mapping
    mEngraveLEMap[0] = ui->leNeck1;
    mEngraveLEMap[1] = ui->leNeck2;
    mEngraveLEMap[2] = ui->leEar1;
    mEngraveLEMap[3] = ui->leEar2;
    mEngraveLEMap[4] = ui->leEar3;
    mEngraveLEMap[5] = ui->leEar4;
    mEngraveLEMap[6] = ui->leRing1;
    mEngraveLEMap[7] = ui->leRing2;
    mEngraveLEMap[8] = ui->leRing3;
    mEngraveLEMap[9] = ui->leRing4;
    mEngraveLEMap[10] = ui->leStone1;
    mEngraveLEMap[11] = ui->leStone2;
    mEngraveLEMap[12] = ui->leEquip1;
    mEngraveLEMap[13] = ui->leEquip2;

    mPenaltyCBMap[0] = ui->cbNeckPenalty;
    mPenaltyCBMap[1] = ui->cbEarPenalty1;
    mPenaltyCBMap[2] = ui->cbEarPenalty2;
    mPenaltyCBMap[3] = ui->cbRingPenalty1;
    mPenaltyCBMap[4] = ui->cbRingPenalty2;
    mPenaltyCBMap[5] = ui->cbStonePenalty;

    // SpinBox Mapping
    mEngraveSPBMap[0] = ui->spbNeck1;
    mEngraveSPBMap[1] = ui->spbNeck2;
    mEngraveSPBMap[2] = ui->spbEar1;
    mEngraveSPBMap[3] = ui->spbEar2;
    mEngraveSPBMap[4] = ui->spbEar3;
    mEngraveSPBMap[5] = ui->spbEar4;
    mEngraveSPBMap[6] = ui->spbRing1;
    mEngraveSPBMap[7] = ui->spbRing2;
    mEngraveSPBMap[8] = ui->spbRing3;
    mEngraveSPBMap[9] = ui->spbRing4;
    mEngraveSPBMap[10] = ui->spbStone1;
    mEngraveSPBMap[11] = ui->spbStone2;
    mEngraveSPBMap[12] = ui->spbEquip1;
    mEngraveSPBMap[13] = ui->spbEquip2;

    mPenaltySPBMap[0] = ui->spbNeckPenalty;
    mPenaltySPBMap[1] = ui->spbEarPenalty1;
    mPenaltySPBMap[2] = ui->spbEarPenalty2;
    mPenaltySPBMap[3] = ui->spbRingPenalty1;
    mPenaltySPBMap[4] = ui->spbRingPenalty2;
    mPenaltySPBMap[5] = ui->spbStonePenalty;
}

void EngraveSimulator::initUI()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont fontNanumBold11 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 11);
    ui->lbLvAtt->setFont(fontNanumBold10);
    ui->lbLvAttSpd->setFont(fontNanumBold10);
    ui->lbLvDef->setFont(fontNanumBold10);
    ui->lbLvSpd->setFont(fontNanumBold10);
    ui->lbEar1->setFont(fontNanumBold11);
    ui->lbEar2->setFont(fontNanumBold11);
    ui->lbEngrave1->setFont(fontNanumBold11);
    ui->lbEngrave2->setFont(fontNanumBold11);
    ui->lbEquip->setFont(fontNanumBold11);
    ui->lbNeck->setFont(fontNanumBold11);
    ui->lbPenalty->setFont(fontNanumBold11);
    ui->lbRing1->setFont(fontNanumBold11);
    ui->lbRing2->setFont(fontNanumBold11);
    ui->lbStone->setFont(fontNanumBold11);
    ui->pbClearAll->setFont(fontNanumBold11);
    ui->lbNameAtt->setFont(fontNanumBold11);
    ui->lbNameAttSpd->setFont(fontNanumBold11);
    ui->lbNameDef->setFont(fontNanumBold11);
    ui->lbNameSpd->setFont(fontNanumBold11);

    ui->groupEngrave->setFont(fontNanumRegular10);
    ui->groupPenalty->setFont(fontNanumRegular10);

    // layout 연결
    ui->groupEngrave->setLayout(mEngraveLayout);

    // PushButton
    ui->pbClearAll->setStyleSheet("QPushButton { color : red }");

    // Label
    ui->lbEngrave1->setStyleSheet("QLabel { border : 1px solid black; color : blue }");
    ui->lbEngrave2->setStyleSheet("QLabel { border : 1px solid black; color : blue }");
    ui->lbPenalty->setStyleSheet("QLabel { border : 1px solid black; color : red }");
    ui->lbNeck->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbEar1->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbEar2->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbRing1->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbRing2->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbStone->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbEquip->setStyleSheet("QLabel { border : 1px solid black }");

    // LineEdit 초기화 (UI, Completer)
    QCompleter* cplEngrave = new QCompleter(mEngrave.getEngraveList(), this);
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        mEngraveLEMap[i]->setFixedWidth(155);
        mEngraveLEMap[i]->setCompleter(cplEngrave);
        mEngraveLEMap[i]->setFont(fontNanumRegular10);
    }
    for (int i = 0; i < mPenaltyCBMap.size(); i++)
    {
        mPenaltyCBMap[i]->setFixedWidth(155);
        mPenaltyCBMap[i]->addItems(mEngrave.getPenaltyList());
        mPenaltyCBMap[i]->setFont(fontNanumRegular10);
    }

    // SpinBox 초기화 (UI)
    for (int i = 0; i < mEngraveSPBMap.size(); i++)
    {
        mEngraveSPBMap[i]->setFixedWidth(45);
        mEngraveSPBMap[i]->setFont(fontNanumRegular10);
    }
    for (int i = 0; i < mPenaltySPBMap.size(); i++)
    {
        mPenaltySPBMap[i]->setFixedWidth(45);
        mPenaltySPBMap[i]->setFont(fontNanumRegular10);
    }
    for (int index = INDEX_ACC_START; index <= INDEX_ACC_END; index++)
    {
        mEngraveSPBMap[index]->setMaximum(MAX_ACC);
    }
    for (int index = INDEX_STONE_START; index <= INDEX_STONE_END; index++)
    {
        mEngraveSPBMap[index]->setMaximum(MAX_STONE);
    }
    for (int index = INDEX_EQUIP_START; index <= INDEX_EQUIP_END; index++)
    {
        mEngraveSPBMap[index]->setMaximum(MAX_EQUIP);
        mEngraveSPBMap[index]->setSingleStep(3);
    }
    for (int index = INDEX_ACC_START; index <= (INDEX_ACC_END / 2); index++)
    {
        mPenaltySPBMap[index]->setMaximum(MAX_ACC_PENALTY);
    }
    mPenaltySPBMap[5]->setMaximum(MAX_STONE);

    // Penalty UI - Pixmap 초기화
    QPixmap att(":/image/resources/engraves/penalty_att.png");
    QPixmap attSpd(":/image/resources/engraves/penalty_attspd.png");
    QPixmap def(":/image/resources/engraves/penalty_def.png");
    QPixmap spd(":/image/resources/engraves/penalty_spd.png");
    ui->lbPixAtt->setPixmap(att.scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->lbPixAttSpd->setPixmap(attSpd.scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->lbPixDef->setPixmap(def.scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->lbPixSpd->setPixmap(spd.scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void EngraveSimulator::initConnect()
{
    connect(ui->pbClearAll, SIGNAL(pressed()), this, SLOT(slotClearInput()));
    for (int i = 0; i < mEngraveSPBMap.size(); i++)
    {
        connect(mEngraveSPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
    for (int i = 0; i < mPenaltySPBMap.size(); i++)
    {
        connect(mPenaltySPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
}

bool EngraveSimulator::validateAccValue()
{
    const int MAX_SUM = 10;
    int value1, value2;

    // 목걸이 수치 검증
    value1 = mEngraveSPBMap[0]->value();
    value2 = mEngraveSPBMap[1]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 귀걸이1 수치 검증
    value1 = mEngraveSPBMap[2]->value();
    value2 = mEngraveSPBMap[3]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 귀걸이2 수치 검증
    value1 = mEngraveSPBMap[4]->value();
    value2 = mEngraveSPBMap[5]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 반지1 수치 검증
    value1 = mEngraveSPBMap[6]->value();
    value2 = mEngraveSPBMap[7]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 반지2 수치 검증
    value1 = mEngraveSPBMap[8]->value();
    value2 = mEngraveSPBMap[9]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    return true;
}

void EngraveSimulator::addEngraveLayout(QString engrave, int value)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* lbPixmap = new QLabel();
    QLabel* lbName = new QLabel();
    QLabel* lbLevel = new QLabel();
    int level = value / 5;
    QString pixPath = mEngrave.getEngravePath(engrave);
    QPixmap pixmap(pixPath);

    lbPixmap->setPixmap(pixmap.scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    lbPixmap->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbLevel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);
    lbName->setFont(fontNanumBold12);
    lbLevel->setFont(fontNanumBold10);
    if (level >= 1)
        lbLevel->setStyleSheet("QLabel { color : blue }");

    lbName->setText(engrave);
    lbLevel->setText(QString("Lv. %1 ( %2 / 15 )").arg(level).arg(value));
    layout->addWidget(lbPixmap);
    layout->addWidget(lbName);
    layout->addWidget(lbLevel);

    mEngraveLayout->addLayout(layout);

    mLabelList.append(lbPixmap);
    mLabelList.append(lbName);
    mLabelList.append(lbLevel);
    mVBoxLayoutList.append(layout);
}

void EngraveSimulator::clearEngraveLayout()
{
    // 결과 layout에 할당된 ui memory 해제
    for (QLabel* label : mLabelList)
        delete label;
    mLabelList.clear();
    for (QVBoxLayout* layout : mVBoxLayoutList)
        delete layout;
    mVBoxLayoutList.clear();
}

void EngraveSimulator::slotClearInput()
{
    // 모든 입력 값 초기화
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        mEngraveLEMap[i]->clear();
    }

    for (int i = 0; i < mEngraveSPBMap.size(); i++)
    {
        mEngraveSPBMap[i]->setValue(0);
    }

    for (int i = 0; i < mPenaltySPBMap.size(); i++)
    {
        mPenaltySPBMap[i]->setValue(0);
    }
}

void EngraveSimulator::slotUpdateResult()
{
    // 결과값 초기화
    mEngrave.clearEngrave();
    mEngrave.clearPenalty();

    // 입력값 검증
    if (!validateAccValue())
    {
        QMessageBox msgBox;
        msgBox.setText("불가능한 각인값입니다. 악세사리 각인값을 확인하세요.");
        msgBox.exec();
        return;
    }

    // 입력값 조회 및 update
    QStringList addedEngraveList;
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        QString engrave = mEngraveLEMap[i]->text();
        if (engrave == "")
            continue;
        else if (mEngrave.isValidEngrave(engrave))
        {
            mEngrave.addEngrave(engrave, mEngraveSPBMap[i]->value());
            if (!addedEngraveList.contains(engrave))
                addedEngraveList << engrave;
        }
        else
        {
            // 각인명이 유효하지 않으면 MessageBox를 띄운 후 update stop
            QMessageBox msgBox;
            msgBox.setText("각인명을 확인하세요.");
            msgBox.exec();
            return;
        }
    }

    clearEngraveLayout();

    // Lv. 내림차순으로 layout에 추가
    for (int level = 3; level >= 0; level--)
    {
        for (const QString& engrave : addedEngraveList)
        {
            int value = mEngrave.getEngraveValue(engrave);
            if (level == (value / 5))
                addEngraveLayout(engrave, value);
        }
    }

    // 감소 각인 update
    for (int i = 0; i < mPenaltyCBMap.size(); i++)
    {
        QString penalty = mPenaltyCBMap[i]->currentText();
        mEngrave.addPenalty(penalty, mPenaltySPBMap[i]->value());
        int value = mEngrave.getPenaltyValue(penalty);
        int level = value / 5;
        if (penalty == "공격력 감소")
        {
            ui->lbLvAtt->setText(QString("Lv. %1 ( %2 / 15 )").arg(level).arg(value));
            if (level >= 1)
                ui->lbLvAtt->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvAtt->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "공격속도 감소")
        {
            ui->lbLvAttSpd->setText(QString("Lv. %1 ( %2 / 15 )").arg(level).arg(value));
            if (level >= 1)
                ui->lbLvAttSpd->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvAttSpd->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "방어력 감소")
        {
            ui->lbLvDef->setText(QString("Lv. %1 ( %2 / 15 )").arg(level).arg(value));
            if (level >= 1)
                ui->lbLvDef->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvDef->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "이동속도 감소")
        {
            ui->lbLvSpd->setText(QString("Lv. %1 ( %2 / 15 )").arg(level).arg(value));
            if (level >= 1)
                ui->lbLvSpd->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvSpd->setStyleSheet("QLabel { color : black }");
        }
    }
}
