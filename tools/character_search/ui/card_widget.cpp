#include "card_widget.h"
#include "ui_card_widget.h"
#include "game_data/card/card.h"
#include "font/font_manager.h"

CardWidget::CardWidget(QWidget* pParent, const Card* pCard) :
    ui(new Ui::CardWidget),
    m_pParent(pParent),
    m_pCard(pCard)
{
    ui->setupUi(this);

    setLabels();
    setFonts();
    setLayoutAlignments();
}

CardWidget::~CardWidget()
{
    for (QLabel* pLabel : m_cardNameLabels)
        delete pLabel;
    m_cardNameLabels.clear();

    for (QLabel* pLabel : m_cardDescLabels)
        delete pLabel;
    m_cardDescLabels.clear();

    delete ui;
}

void CardWidget::setLabels()
{
    const QStringList& effectNames = m_pCard->getEffectNames();
    const QStringList& effectDescs = m_pCard->getEffectDescs();

    for (int i = 0; i < effectNames.size(); i++)
    {
        QLabel* pLbName = createNameLabel(effectNames[i]);
        ui->vLayoutCard->addWidget(pLbName);

        QLabel* pLbDesc = createDescLabel(effectDescs[i]);
        ui->vLayoutCard->addWidget(pLbDesc);
    }
}

QLabel* CardWidget::createNameLabel(const QString& name)
{
    QLabel* pLabel = new QLabel(name);
    pLabel->setStyleSheet("QLabel { color: #00B700 }");
    m_cardNameLabels.append(pLabel);
    return pLabel;
}

QLabel* CardWidget::createDescLabel(const QString& desc)
{
    QLabel* pLabel = new QLabel(desc);
    pLabel->setWordWrap(true);
    pLabel->setFixedHeight(30);
    pLabel->setAlignment(Qt::AlignTop);
    m_cardDescLabels.append(pLabel);
    return pLabel;
}

void CardWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    for (QLabel* pLabel : m_cardNameLabels)
        pLabel->setFont(nanumBold10);
    for (QLabel* pLabel : m_cardDescLabels)
        pLabel->setFont(nanumBold10);
    ui->groupCard->setFont(nanumRegular10);
}

void CardWidget::setLayoutAlignments()
{
    ui->vLayoutCard->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    for (QLabel* pLabel : m_cardNameLabels)
        pLabel->setAlignment(Qt::AlignHCenter);
    for (QLabel* pLabel : m_cardDescLabels)
        pLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}
