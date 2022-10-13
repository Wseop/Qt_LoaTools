#include "EngraveSimulator.h"
#include "ui_engravesimulator.h"

#include <QCompleter>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

EngraveSimulator::EngraveSimulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EngraveSimulator),
    mEngraveLayout(new QHBoxLayout())
{
    ui->setupUi(this);

    loadEngraveList();

    initMap();
    initUI();
    initConnect();
}

EngraveSimulator::~EngraveSimulator()
{
    delete ui;
}

void EngraveSimulator::loadEngraveList()
{
    // 각인 리스트 불러오기
    QString filePath = QDir::currentPath() + "/resources/EngraveList.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << ": File Open Fail";
        return;
    }

    QTextStream inStream(&file);
    while (!inStream.atEnd())
    {
        mEngraveList << inStream.readLine();
    }

    // 페널티 리스트 초기화
    mPenaltyList << "공격력 감소" << "공격속도 감소" << "방어력 감소" << "이동속도 감소";
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
    // layout 연결
    ui->groupEngrave->setLayout(mEngraveLayout);

    // PushButton
    ui->pbHome->setIcon(QIcon(":resources/Home.ico"));
    ui->pbHome->setIconSize(QSize(30, 30));
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
    QCompleter* cplEngrave = new QCompleter(mEngraveList, this);
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        mEngraveLEMap[i]->setFixedWidth(155);
        mEngraveLEMap[i]->setCompleter(cplEngrave);
    }
    for (int i = 0; i < mPenaltyCBMap.size(); i++)
    {
        mPenaltyCBMap[i]->setFixedWidth(155);
        mPenaltyCBMap[i]->addItems(mPenaltyList);
    }

    // SpinBox 초기화 (UI)
    for (int i = 0; i < mEngraveSPBMap.size(); i++)
    {
        mEngraveSPBMap[i]->setFixedWidth(45);
    }
    for (int i = 0; i < mPenaltySPBMap.size(); i++)
    {
        mPenaltySPBMap[i]->setFixedWidth(45);
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
}

void EngraveSimulator::initConnect()
{
    connect(ui->pbHome, SIGNAL(pressed()), this, SLOT(slotHome()));
    connect(ui->pbClearAll, SIGNAL(pressed()), this, SLOT(slotClearAll()));
    for (int i = 0; i < mEngraveSPBMap.size(); i++)
    {
        connect(mEngraveSPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
    for (int i = 0; i < mPenaltySPBMap.size(); i++)
    {
        connect(mPenaltySPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
}

bool EngraveSimulator::isValidEngrave(QString engrave)
{
    // 입력된 각인명 검사
    if (mEngraveList.contains(engrave))
        return true;
    else
        return false;
}

void EngraveSimulator::addEngraveLayout(QString engrave, int value)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* lbName = new QLabel();
    QLabel* lbValue = new QLabel();
    QLabel* lbLevel = new QLabel();
    int level = value / 5;

    lbName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbValue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lbLevel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    lbName->setFont(QFont("나눔스퀘어 네오 Bold", 12, QFont::Bold));
    lbValue->setFont(QFont("나눔스퀘어 네오 Bold", 10, QFont::Bold));
    lbLevel->setFont(QFont("나눔스퀘어 네오 Bold", 10, QFont::Bold));
    if (level >= 1)
        lbLevel->setStyleSheet("QLabel { color : blue }");

    lbName->setText(engrave);
    lbValue->setText(QString("(%1 / 15)").arg(value));
    lbLevel->setText(QString("Lv. %1").arg(level));
    layout->addWidget(lbName);
    layout->addWidget(lbValue);
    layout->addWidget(lbLevel);

    mEngraveLayout->addLayout(layout);

    mLabelList.append(lbName);
    mLabelList.append(lbValue);
    mLabelList.append(lbLevel);
    mVBoxLayoutList.append(layout);
}

void EngraveSimulator::slotHome()
{
    QWidget* parent = static_cast<QWidget*>(this->parent());
    parent->setFixedWidth(800);
    parent->setFixedHeight(600);

    this->close();
}

void EngraveSimulator::slotClearAll()
{
    // 모든 입력 값 초기화
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        mEngraveLEMap[i]->clear();
    }

//    for (int i = 0; i < mPenaltyCBMap.size(); i++)
//    {
//        mPenaltyCBMap[i]->clearEditText();
//    }

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
    // 기존값 초기화
    mEngraveValueMap.clear();
    mPenaltyValueMap.clear();

    // 결과 layout에 할당된 ui memory 해제
    for (QLabel* label : mLabelList)
        delete label;
    mLabelList.clear();
    for (QVBoxLayout* layout : mVBoxLayoutList)
        delete layout;
    mVBoxLayoutList.clear();

    // 입력값 조회 및 update
    QStringList engraveList;
    for (int i = 0; i < mEngraveLEMap.size(); i++)
    {
        QString engrave = mEngraveLEMap[i]->text();
        if (engrave == "")
            continue;
        else if (isValidEngrave(engrave))
        {
            mEngraveValueMap[engrave] += mEngraveSPBMap[i]->value();
            if (!engraveList.contains(engrave))
                engraveList << engrave;
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

    // Lv. 내림차순으로 layout에 추가
    for (int level = 3; level >= 0; level--)
    {
        for (const QString& engrave : engraveList)
        {
            if (level == (mEngraveValueMap[engrave] / 5))
            {
                addEngraveLayout(engrave, mEngraveValueMap[engrave]);
            }
        }
    }

    // 감소 각인 update
    for (int i = 0; i < mPenaltyCBMap.size(); i++)
    {
        QString penalty = mPenaltyCBMap[i]->currentText();
        mPenaltyValueMap[penalty] += mPenaltySPBMap[i]->value();
        int value = mPenaltyValueMap[penalty];
        int level = value / 5;
        if (penalty == "공격력 감소")
        {
            ui->lbValueAtt->setText(QString("( %1 / 15 )").arg(value));
            ui->lbLvAtt->setText(QString("Lv. %1").arg(level));
            if (level >= 1)
                ui->lbLvAtt->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvAtt->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "공격속도 감소")
        {
            ui->lbValueAttSpd->setText(QString("( %1 / 15 )").arg(mPenaltyValueMap[penalty]));
            ui->lbLvAttSpd->setText(QString("Lv. %1").arg(level));
            if (level >= 1)
                ui->lbLvAttSpd->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvAttSpd->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "방어력 감소")
        {
            ui->lbValueDef->setText(QString("( %1 / 15 )").arg(mPenaltyValueMap[penalty]));
            ui->lbLvDef->setText(QString("Lv. %1").arg(level));
            if (level >= 1)
                ui->lbLvDef->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvDef->setStyleSheet("QLabel { color : black }");
        }
        else if (penalty == "이동속도 감소")
        {
            ui->lbValueSpd->setText(QString("( %1 / 15 )").arg(mPenaltyValueMap[penalty]));
            ui->lbLvSpd->setText(QString("Lv. %1").arg(level));
            if (level >= 1)
                ui->lbLvSpd->setStyleSheet("QLabel { color : red }");
            else
                ui->lbLvSpd->setStyleSheet("QLabel { color : black }");
        }
    }
}
























