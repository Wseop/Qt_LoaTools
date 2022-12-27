#include "engrave_widget.h"
#include "ui_engrave_widget.h"
#include "game_data/engrave/engrave.h"
#include "game_data/engrave/engrave_manager.h"
#include "font/font_manager.h"

#include <QPixmap>

EngraveWidget::EngraveWidget(QWidget* pParent, const Engrave* pEngrave) :
    ui(new Ui::EngraveWidget),
    m_pParent(pParent),
    m_pEngrave(pEngrave)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("각인");

    setEngraveLayouts();
    setFonts();
    setLayoutAlignments();
}

EngraveWidget::~EngraveWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    m_labels.clear();

    for (QHBoxLayout* pLayout : m_hLayouts)
        delete pLayout;
    m_hLayouts.clear();

    delete ui;
}

void EngraveWidget::setEngraveLayouts()
{
    const auto& engraves = m_pEngrave->getEngraves();
    for (const QPair<QString, int>& engrave : engraves)
    {
        QHBoxLayout* pLayout = createHLayout();
        ui->vLayoutEngraves->addLayout(pLayout);

        QLabel* pIconLabel = createIconLabel(EngraveManager::getInstance()->getEngravePath(engrave.first));
        pLayout->addWidget(pIconLabel);
        QLabel* pNameLabel = createNameLabel(engrave.first);
        pLayout->addWidget(pNameLabel);
        QLabel* pLevelLabel = createLevelLabel(engrave.second);
        pLayout->addWidget(pLevelLabel);
    }

    QString penaltyLabelColor = QString("QLabel { color: red }");
    const auto& penalties = m_pEngrave->getPenalties();
    for (const QPair<QString, int>& penalty : penalties)
    {
        QHBoxLayout* pLayout = createHLayout();
        ui->vLayoutEngraves->addLayout(pLayout);

        QLabel* pIconLabel = createIconLabel(EngraveManager::getInstance()->getPenaltyPath(penalty.first));
        pLayout->addWidget(pIconLabel);
        QLabel* pNameLabel = createNameLabel(penalty.first);
        pNameLabel->setStyleSheet(penaltyLabelColor);
        pLayout->addWidget(pNameLabel);
        QLabel* pLevelLabel = createLevelLabel(penalty.second);
        pLevelLabel->setStyleSheet(penaltyLabelColor);
        pLayout->addWidget(pLevelLabel);
    }
}

QHBoxLayout* EngraveWidget::createHLayout()
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    pLayout->setSpacing(9);
    m_hLayouts.append(pLayout);
    return pLayout;
}

void EngraveWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    for (QLabel* pLabel : m_labels)
        pLabel->setFont(nanumBold10);
    ui->groupEngrave->setFont(nanumRegular10);
}

QLabel* EngraveWidget::createIconLabel(QString iconPath)
{
    QLabel* pLabel = new QLabel();
    pLabel->setFixedSize(50, 50);
    m_labels.append(pLabel);
    QPixmap icon;
    if (icon.load(iconPath))
    {
        pLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        pLabel->setStyleSheet("QLabel { border: 1px solid black }");
    }
    return pLabel;
}

QLabel* EngraveWidget::createNameLabel(QString engrave)
{
    QLabel* pLabel = new QLabel(engrave);
    m_labels.append(pLabel);
    return pLabel;
}

QLabel* EngraveWidget::createLevelLabel(int level)
{
    QString labelText = QString("Lv.%1").arg(level);
    QLabel* pLabel = new QLabel(labelText);
    m_labels.append(pLabel);
    return pLabel;
}

void EngraveWidget::setLayoutAlignments()
{
    ui->vLayoutEngraves->setAlignment(Qt::AlignVCenter);
}
