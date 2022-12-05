#include "engrave_widget.h"
#include "ui_engrave_widget.h"
#include "game_data/engrave.h"
#include "font/font_manager.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

EngraveWidget::EngraveWidget(const QMap<QString, int>& engraveValues, const QMap<QString, int>& penaltyValues) :
    ui(new Ui::EngraveWidget),
    m_engraveValues(engraveValues),
    m_penaltyValues(penaltyValues)
{
    ui->setupUi(this);

    initFont();

    createEngraveLayouts(Engrave::getInstance()->extractActiveEngraves(m_engraveValues));
    createPenaltyLayouts(Engrave::getInstance()->extractActivePenalties(m_penaltyValues));
}

EngraveWidget::~EngraveWidget()
{
    for (QLabel* label : m_labels)
        delete label;
    m_labels.clear();

    for (QHBoxLayout* layout : m_engraveLayouts)
    {
        ui->vLayoutEngrave->removeItem(layout);
        delete layout;
    }
    m_engraveLayouts.clear();

    for (QHBoxLayout* layout : m_penaltyLayouts)
    {
        ui->vLayoutPenalty->removeItem(layout);
        delete layout;
    }
    m_penaltyLayouts.clear();

    delete ui;
}

void EngraveWidget::initFont()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    ui->groupEngrave->setFont(fontNanumRegular10);
}

void EngraveWidget::createEngraveLayouts(const QStringList& engraves)
{
    for (int level = 3; level > 0; level--)
    {
        for (const QString& engrave : engraves)
        {
            int engraveLevel = m_engraveValues[engrave] / 5;
            if (engraveLevel != level)
                continue;

            QString iconPath = Engrave::getInstance()->getEngravePath(engrave);
            QHBoxLayout* engraveLayout = createEngraveLayout(iconPath, engrave, level);
            ui->vLayoutEngrave->addLayout(engraveLayout);
            m_engraveLayouts.append(engraveLayout);
        }
    }
}

void EngraveWidget::createPenaltyLayouts(const QStringList &penalties)
{
    for (int level = 3; level > 0; level--)
    {
        for (const QString& penalty : penalties)
        {
            int penaltyLevel = m_penaltyValues[penalty] / 5;
            if (penaltyLevel != level)
                continue;

            QString iconPath = Engrave::getInstance()->getPenaltyPath(penalty);
            QHBoxLayout* penaltyLayout = createEngraveLayout(iconPath, penalty, level);
            ui->vLayoutPenalty->addLayout(penaltyLayout);
            m_penaltyLayouts.append(penaltyLayout);
        }
    }
}

QHBoxLayout* EngraveWidget::createEngraveLayout(QString iconPath, QString engrave, int level)
{
    QHBoxLayout* engraveLayout = new QHBoxLayout();

    QLabel* lbIcon = new QLabel();
    lbIcon->setFixedSize(50, 50);
    engraveLayout->addWidget(lbIcon);
    m_labels.append(lbIcon);
    QPixmap icon;
    if (!icon.load(iconPath))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
    }
    else
    {
        lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    QLabel* lbEngraveName = new QLabel(engrave);
    lbEngraveName->setFont(fontNanumBold10);
    lbEngraveName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    engraveLayout->addWidget(lbEngraveName);
    m_labels.append(lbEngraveName);

    QLabel* lbEngraveValue = new QLabel(QString("Lv. %1").arg(level));
    lbEngraveValue->setFont(fontNanumBold10);
    lbEngraveValue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    engraveLayout->addWidget(lbEngraveValue);
    m_labels.append(lbEngraveValue);

    if (engrave.contains("감소"))
    {
        lbEngraveName->setStyleSheet(QString("QLabel { color: red }"));
        lbEngraveValue->setStyleSheet(QString("QLabel { color: red }"));
    }

    return engraveLayout;
}
