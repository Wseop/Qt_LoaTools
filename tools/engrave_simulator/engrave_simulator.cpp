#include "engrave_simulator.h"
#include "ui_engravesimulator.h"
#include "font/font_manager.h"
#include "game_data/engrave/engrave_manager.h"

#include <QCompleter>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

EngraveSimulator* EngraveSimulator::m_pEngraveSimulator = nullptr;

EngraveSimulator::EngraveSimulator() :
    ui(new Ui::EngraveSimulator),
    m_pEngraveLayout(new QHBoxLayout())
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
    m_engraveLEMap[0] = ui->leNeck1;
    m_engraveLEMap[1] = ui->leNeck2;
    m_engraveLEMap[2] = ui->leEar1;
    m_engraveLEMap[3] = ui->leEar2;
    m_engraveLEMap[4] = ui->leEar3;
    m_engraveLEMap[5] = ui->leEar4;
    m_engraveLEMap[6] = ui->leRing1;
    m_engraveLEMap[7] = ui->leRing2;
    m_engraveLEMap[8] = ui->leRing3;
    m_engraveLEMap[9] = ui->leRing4;
    m_engraveLEMap[10] = ui->leStone1;
    m_engraveLEMap[11] = ui->leStone2;
    m_engraveLEMap[12] = ui->leEquip1;
    m_engraveLEMap[13] = ui->leEquip2;

    m_penaltyCBMap[0] = ui->cbNeckPenalty;
    m_penaltyCBMap[1] = ui->cbEarPenalty1;
    m_penaltyCBMap[2] = ui->cbEarPenalty2;
    m_penaltyCBMap[3] = ui->cbRingPenalty1;
    m_penaltyCBMap[4] = ui->cbRingPenalty2;
    m_penaltyCBMap[5] = ui->cbStonePenalty;

    // SpinBox Mapping
    m_engraveSPBMap[0] = ui->spbNeck1;
    m_engraveSPBMap[1] = ui->spbNeck2;
    m_engraveSPBMap[2] = ui->spbEar1;
    m_engraveSPBMap[3] = ui->spbEar2;
    m_engraveSPBMap[4] = ui->spbEar3;
    m_engraveSPBMap[5] = ui->spbEar4;
    m_engraveSPBMap[6] = ui->spbRing1;
    m_engraveSPBMap[7] = ui->spbRing2;
    m_engraveSPBMap[8] = ui->spbRing3;
    m_engraveSPBMap[9] = ui->spbRing4;
    m_engraveSPBMap[10] = ui->spbStone1;
    m_engraveSPBMap[11] = ui->spbStone2;
    m_engraveSPBMap[12] = ui->spbEquip1;
    m_engraveSPBMap[13] = ui->spbEquip2;

    m_penaltySPBMap[0] = ui->spbNeckPenalty;
    m_penaltySPBMap[1] = ui->spbEarPenalty1;
    m_penaltySPBMap[2] = ui->spbEarPenalty2;
    m_penaltySPBMap[3] = ui->spbRingPenalty1;
    m_penaltySPBMap[4] = ui->spbRingPenalty2;
    m_penaltySPBMap[5] = ui->spbStonePenalty;
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
    ui->groupEngrave->setLayout(m_pEngraveLayout);

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
    QCompleter* cplEngrave = new QCompleter(EngraveManager::getInstance()->getEngraveList(), this);
    for (int i = 0; i < m_engraveLEMap.size(); i++)
    {
        m_engraveLEMap[i]->setFixedWidth(155);
        m_engraveLEMap[i]->setCompleter(cplEngrave);
        m_engraveLEMap[i]->setFont(fontNanumRegular10);
    }
    for (int i = 0; i < m_penaltyCBMap.size(); i++)
    {
        m_penaltyCBMap[i]->setFixedWidth(155);
        m_penaltyCBMap[i]->addItems(EngraveManager::getInstance()->getPenaltyList());
        m_penaltyCBMap[i]->setFont(fontNanumRegular10);
    }

    // SpinBox 초기화 (UI)
    for (int i = 0; i < m_engraveSPBMap.size(); i++)
    {
        m_engraveSPBMap[i]->setFixedWidth(45);
        m_engraveSPBMap[i]->setFont(fontNanumRegular10);
    }
    for (int i = 0; i < m_penaltySPBMap.size(); i++)
    {
        m_penaltySPBMap[i]->setFixedWidth(45);
        m_penaltySPBMap[i]->setFont(fontNanumRegular10);
    }
    for (int index = INDEX_ACC_START; index <= INDEX_ACC_END; index++)
    {
        m_engraveSPBMap[index]->setMaximum(MAX_ACC);
    }
    for (int index = INDEX_STONE_START; index <= INDEX_STONE_END; index++)
    {
        m_engraveSPBMap[index]->setMaximum(MAX_STONE);
    }
    for (int index = INDEX_EQUIP_START; index <= INDEX_EQUIP_END; index++)
    {
        m_engraveSPBMap[index]->setMaximum(MAX_EQUIP);
        m_engraveSPBMap[index]->setSingleStep(3);
    }
    for (int index = INDEX_ACC_START; index <= (INDEX_ACC_END / 2); index++)
    {
        m_penaltySPBMap[index]->setMaximum(MAX_ACC_PENALTY);
    }
    m_penaltySPBMap[5]->setMaximum(MAX_STONE);

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
    for (int i = 0; i < m_engraveSPBMap.size(); i++)
    {
        connect(m_engraveSPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
    for (int i = 0; i < m_penaltySPBMap.size(); i++)
    {
        connect(m_penaltySPBMap[i], SIGNAL(valueChanged(int)), this, SLOT(slotUpdateResult()));
    }
}

bool EngraveSimulator::validateAccValue()
{
    const int MAX_SUM = 10;
    int value1, value2;

    // 목걸이 수치 검증
    value1 = m_engraveSPBMap[0]->value();
    value2 = m_engraveSPBMap[1]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 귀걸이1 수치 검증
    value1 = m_engraveSPBMap[2]->value();
    value2 = m_engraveSPBMap[3]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 귀걸이2 수치 검증
    value1 = m_engraveSPBMap[4]->value();
    value2 = m_engraveSPBMap[5]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 반지1 수치 검증
    value1 = m_engraveSPBMap[6]->value();
    value2 = m_engraveSPBMap[7]->value();
    if (value1 + value2 >= MAX_SUM)
        return false;

    // 반지2 수치 검증
    value1 = m_engraveSPBMap[8]->value();
    value2 = m_engraveSPBMap[9]->value();
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
    QString pixPath = EngraveManager::getInstance()->getEngravePath(engrave);
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

    m_pEngraveLayout->addLayout(layout);

    m_labelList.append(lbPixmap);
    m_labelList.append(lbName);
    m_labelList.append(lbLevel);
    m_vBoxLayoutList.append(layout);
}

void EngraveSimulator::clearEngraveLayout()
{
    // 결과 layout에 할당된 ui memory 해제
    for (QLabel* label : m_labelList)
        delete label;
    m_labelList.clear();
    for (QVBoxLayout* layout : m_vBoxLayoutList)
        delete layout;
    m_vBoxLayoutList.clear();
}

void EngraveSimulator::slotClearInput()
{
    // 모든 입력 값 초기화
    for (int i = 0; i < m_engraveLEMap.size(); i++)
    {
        m_engraveLEMap[i]->clear();
    }

    for (int i = 0; i < m_engraveSPBMap.size(); i++)
    {
        m_engraveSPBMap[i]->setValue(0);
    }

    for (int i = 0; i < m_penaltySPBMap.size(); i++)
    {
        m_penaltySPBMap[i]->setValue(0);
    }
}

void EngraveSimulator::slotUpdateResult()
{
    // 결과값 초기화
    m_engraveValue.clear();
    m_penaltyValue.clear();

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
    for (int i = 0; i < m_engraveLEMap.size(); i++)
    {
        QString engrave = m_engraveLEMap[i]->text();
        if (engrave == "")
            continue;
        else if (EngraveManager::getInstance()->isValidEngrave(engrave))
        {
            m_engraveValue[engrave] += m_engraveSPBMap[i]->value();
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
            int value = m_engraveValue[engrave];
            if (level == (value / 5))
                addEngraveLayout(engrave, value);
        }
    }

    // 감소 각인 update
    for (int i = 0; i < m_penaltyCBMap.size(); i++)
    {
        QString penalty = m_penaltyCBMap[i]->currentText();
        m_penaltyValue[penalty] += m_penaltySPBMap[i]->value();
        int value = m_penaltyValue[penalty];
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
