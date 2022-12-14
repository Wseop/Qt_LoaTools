#include "engrave_widget.h"
#include "ui_engrave_widget.h"
#include "game_data/engrave/engrave.h"
#include "game_data/engrave/engrave_manager.h"
#include "font/font_manager.h"

#include <QPixmap>

EngraveWidget::EngraveWidget(QWidget *parent, const Engrave* engrave) :
    ui(new Ui::EngraveWidget),
    m_pParent(parent),
    m_pEngrave(engrave)
{
    ui->setupUi(this);

    addEngraveLayouts();
    setFonts();
    setAlignments();
    setStyleSheets();
}

EngraveWidget::~EngraveWidget()
{
    for (QLabel* label : m_engraveLabels)
        delete label;
    m_engraveLabels.clear();

    for (QLabel* label : m_penaltyLabels)
        delete label;
    m_penaltyLabels.clear();

    for (QHBoxLayout* layout : m_hLayouts)
        delete layout;
    m_hLayouts.clear();

    delete ui;
}

void EngraveWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    for (QLabel* label : m_engraveLabels)
        label->setFont(nanumBold10);
    for (QLabel* label : m_penaltyLabels)
        label->setFont(nanumBold10);
    ui->groupEngrave->setFont(nanumRegular10);
}

void EngraveWidget::setAlignments()
{
    ui->vLayoutEngraves->setAlignment(Qt::AlignTop);
    for (QHBoxLayout* layout : m_hLayouts)
        layout->setAlignment(Qt::AlignVCenter);
    for (QLabel* label : m_engraveLabels)
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    for (QLabel* label : m_penaltyLabels)
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void EngraveWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");

    for (QLabel* label : m_penaltyLabels)
        label->setStyleSheet(labelColor.arg("red"));
}

void EngraveWidget::addEngraveLayouts()
{
    const QList<QPair<QString, int>>& engraves = m_pEngrave->getEngraves();
    for (const auto& engrave : engraves)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        m_hLayouts.append(layout);
        ui->vLayoutEngraves->addLayout(layout);

        QLabel* lbIcon = createIcon(engrave.first);
        QLabel* lbName = createName(engrave.first);
        QLabel* lbLevel = createLevel(engrave.second);
        m_engraveLabels.append(lbIcon);
        m_engraveLabels.append(lbName);
        m_engraveLabels.append(lbLevel);
        layout->addWidget(lbIcon);
        layout->addWidget(lbName);
        layout->addWidget(lbLevel);
    }

    const QList<QPair<QString, int>>& penalties = m_pEngrave->getPenalties();
    for (const auto& penalty : penalties)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        m_hLayouts.append(layout);
        ui->vLayoutEngraves->addLayout(layout);

        QLabel* lbIcon = createIcon(penalty.first);
        QLabel* lbName = createName(penalty.first);
        QLabel* lbLevel = createLevel(penalty.second);
        m_penaltyLabels.append(lbIcon);
        m_penaltyLabels.append(lbName);
        m_penaltyLabels.append(lbLevel);
        layout->addWidget(lbIcon);
        layout->addWidget(lbName);
        layout->addWidget(lbLevel);
    }
}

QLabel* EngraveWidget::createIcon(QString engrave)
{
    QLabel* label = new QLabel();

    QString iconPath = EngraveManager::getInstance()->getEngravePath(engrave);
    QPixmap icon;
    if (!icon.load(iconPath))
        return label;
    label->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    return label;
}

QLabel* EngraveWidget::createName(QString engrave)
{
    QLabel* label = new QLabel(engrave);
    return label;
}

QLabel* EngraveWidget::createLevel(int level)
{
    QString labelText = QString("Lv.%1").arg(level);
    QLabel* label = new QLabel(labelText);
    return label;
}
