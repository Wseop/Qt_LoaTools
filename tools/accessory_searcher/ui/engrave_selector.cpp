#include "engrave_selector.h"
#include "ui_engrave_selector.h"
#include "game_data/engrave/engrave_manager.h"
#include "font/font_manager.h"
#include "tools/accessory_searcher/accessory_searcher.h"

#include <functional>
#include <QPushButton>

EngraveSelector::EngraveSelector(int buttonIndex) :
    ui(new Ui::EngraveSelector),
    m_buttonIndex(buttonIndex),
    m_pos({0, 0})
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("각인 선택");

    setEngravings();
    setEngravingButtons();
}

EngraveSelector::~EngraveSelector()
{
    for (auto* pBtn : m_btns)
        delete pBtn;
    delete ui;
}

void EngraveSelector::setEngravings()
{
    EngraveManager* pEngraveManager = EngraveManager::getInstance();

    const QStringList& engravings = pEngraveManager->getEngraveList();
    for (const QString& engrave : engravings)
    {
        if (pEngraveManager->isClassEngrave(engrave))
        {
            m_classEngravings << engrave;
        }
        else
        {
            m_engravings << engrave;
        }
    }

    std::sort(m_engravings.begin(), m_engravings.end(), [&](const QString& a, const QString& b){
        return a < b;
    });
    std::sort(m_classEngravings.begin(), m_classEngravings.end(), [&](const QString& a, const QString& b){
        return a < b;
    });
}

void EngraveSelector::setEngravingButtons()
{
    {
        QPushButton* pButton = createButton("선택 안함");
        connect(pButton, &QPushButton::pressed, this, [&](){
            AccessorySearcher::getInstance()->setEngraving(m_buttonIndex, "");
            this->close();
        });
        ui->gridCancle->addWidget(pButton, 0, 0);
    }

    for (const QString& engraving : m_engravings)
    {
        QPushButton* pButton = createButton(engraving);
        connect(pButton, &QPushButton::pressed, this, [&, engraving](){
            AccessorySearcher::getInstance()->setEngraving(m_buttonIndex, engraving);
            this->close();
        });
        ui->gridEngraving->addWidget(pButton, m_pos.row, m_pos.col);
        movePos();
    }

    m_pos = {0, 0};
    for (const QString& engraving : m_classEngravings)
    {
        QPushButton* pButton = createButton(engraving);
        connect(pButton, &QPushButton::pressed, this, [&, engraving](){
            AccessorySearcher::getInstance()->setEngraving(m_buttonIndex, engraving);
            this->close();
        });
        ui->gridClassEngraving->addWidget(pButton, m_pos.row, m_pos.col);
        movePos();
    }
}

QPushButton* EngraveSelector::createButton(QString text)
{
    QPushButton* pButton = new QPushButton(text);
    pButton->setFixedSize(125, 35);
    pButton->setFont(FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10));
    m_btns.append(pButton);
    return pButton;
}

void EngraveSelector::movePos()
{
    m_pos.col++;
    if (m_pos.col == MAX_COL)
    {
        m_pos.row++;
        m_pos.col = 0;
    }
}
