#include "penalty_selector.h"
#include "ui_penalty_selector.h"
#include "game_data/engrave/engrave_manager.h"
#include "font/font_manager.h"

#include <QPushButton>

PenaltySelector::PenaltySelector() :
    ui(new Ui::PenaltySelector)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("각인 선택");

    setPenalties();
    setPenaltyButtons();
}

PenaltySelector::~PenaltySelector()
{
    for (auto* pBtn : m_btns)
        delete pBtn;
    delete ui;
}

void PenaltySelector::setPenalties()
{
    const QStringList& penalties = EngraveManager::getInstance()->getPenaltyList();
    for (const QString& penalty : penalties)
    {
        m_penalties << penalty;
    }
}

void PenaltySelector::setPenaltyButtons()
{
    for (const QString& penalty : m_penalties)
    {
        QPushButton* pButton = createButton(penalty);
        // TODO. connect
        ui->hLayoutPenalty->addWidget(pButton);
    }
}

QPushButton* PenaltySelector::createButton(QString text)
{
    QPushButton* pButton = new QPushButton(text);
    pButton->setFixedSize(125, 35);
    pButton->setFont(FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10));
    m_btns.append(pButton);
    return pButton;
}
