#include "penalty_selector.h"
#include "ui_penalty_selector.h"
#include "game_data/engrave/engrave_manager.h"
#include "font/font_manager.h"
#include "tools/accessory_searcher/accessory_searcher.h"

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
    {
        QPushButton* pButton = createButton("선택 안함");
        connect(pButton, &QPushButton::pressed, this, [&](){
            AccessorySearcher::getInstance()->setPenalty("");
            this->close();
        });
        ui->hLayoutCancle->addWidget(pButton);
    }

    for (const QString& penalty : m_penalties)
    {
        QPushButton* pButton = createButton(penalty);
        connect(pButton, &QPushButton::pressed, this, [&, penalty](){
            AccessorySearcher::getInstance()->setPenalty(penalty);
            this->close();
        });
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
