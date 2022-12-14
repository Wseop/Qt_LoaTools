#include "card_widget.h"
#include "ui_card_widget.h"
#include "game_data/card/card.h"
#include "font/font_manager.h"

CardWidget::CardWidget(QWidget* parent, const Card* card) :
    ui(new Ui::CardWidget),
    m_pParent(parent),
    m_pCard(card)
{
    ui->setupUi(this);

    addCards();
    setFonts();
    setAlignments();
    setStyleSheets();
}

CardWidget::~CardWidget()
{
    for (QLabel* label : m_cardNameLabels)
        delete label;
    m_cardNameLabels.clear();

    for (QLabel* label : m_cardDescLabels)
        delete label;
    m_cardDescLabels.clear();

    delete ui;
}

void CardWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    for (QLabel* label : m_cardNameLabels)
        label->setFont(nanumBold10);
    for (QLabel* label : m_cardDescLabels)
        label->setFont(nanumBold10);
    ui->groupCard->setFont(nanumRegular10);
}

void CardWidget::setAlignments()
{
    ui->vLayoutCard->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    for (QLabel* label : m_cardNameLabels)
        label->setAlignment(Qt::AlignHCenter);
    for (QLabel* label : m_cardDescLabels)
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void CardWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");

    for (QLabel* label : m_cardNameLabels)
        label->setStyleSheet(labelColor.arg("#00B700"));
}

void CardWidget::addCards()
{
    const QStringList& effectNames = m_pCard->getEffectNames();
    const QStringList& effectDescs = m_pCard->getEffectDescs();

    for (int i = 0; i < effectNames.size(); i++)
    {
        QLabel* lbName = createName(effectNames[i]);
        QLabel* lbDesc = createDesc(effectDescs[i]);
        ui->vLayoutCard->addWidget(lbName);
        ui->vLayoutCard->addWidget(lbDesc);
    }
}

QLabel* CardWidget::createName(const QString& name)
{
    QLabel* label = new QLabel(name);
    m_cardNameLabels.append(label);
    return label;
}

QLabel* CardWidget::createDesc(const QString& desc)
{
    QLabel* label = new QLabel(desc);
    label->setWordWrap(true);
    label->setFixedHeight(30);
    m_cardDescLabels.append(label);
    return label;
}
