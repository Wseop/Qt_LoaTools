#include "engrave_widget.h"
#include "ui_engrave_widget.h"
#include "engrave.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

EngraveWidget::EngraveWidget(QWidget *parent, const Engrave* engrave) :
    QWidget(parent),
    ui(new Ui::EngraveWidget),
    mEngrave(engrave)
{
    ui->setupUi(this);

    createEngraveLayouts(engrave->getActiveEngraveList());
    createPenaltyLayouts(engrave->getActivePenaltyList());
}

EngraveWidget::~EngraveWidget()
{
    for (QLabel* label : mLabels)
        delete label;
    mLabels.clear();

    for (QHBoxLayout* layout : mEngraveLayouts)
    {
        ui->vLayoutEngrave->removeItem(layout);
        delete layout;
    }
    mEngraveLayouts.clear();

    for (QHBoxLayout* layout : mPenaltyLayouts)
    {
        ui->vLayoutPenalty->removeItem(layout);
        delete layout;
    }
    mPenaltyLayouts.clear();

    delete ui;
}

void EngraveWidget::createEngraveLayouts(const QStringList& engraves)
{
    for (int level = 3; level > 0; level--)
    {
        for (const QString& engrave : engraves)
        {
            int engraveLevel = mEngrave->getEngraveValue(engrave) / 5;
            if (engraveLevel != level)
                continue;

            QString iconPath = mEngrave->getEngravePath(engrave);
            QHBoxLayout* engraveLayout = createEngraveLayout(iconPath, engrave, level);
            ui->vLayoutEngrave->addLayout(engraveLayout);
            mEngraveLayouts.append(engraveLayout);
        }
    }
}

void EngraveWidget::createPenaltyLayouts(const QStringList &penalties)
{
    for (int level = 3; level > 0; level--)
    {
        for (const QString& penalty : penalties)
        {
            int penaltyLevel = mEngrave->getPenaltyValue(penalty) / 5;
            if (penaltyLevel != level)
                continue;

            QString iconPath = mEngrave->getPenaltyPath(penalty);
            QHBoxLayout* penaltyLayout = createEngraveLayout(iconPath, penalty, level);
            ui->vLayoutPenalty->addLayout(penaltyLayout);
            mPenaltyLayouts.append(penaltyLayout);
        }
    }
}

QHBoxLayout* EngraveWidget::createEngraveLayout(QString iconPath, QString engrave, int level)
{
    QHBoxLayout* engraveLayout = new QHBoxLayout();

    QLabel* lbIcon = new QLabel();
    lbIcon->setFixedSize(50, 50);
    lbIcon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    engraveLayout->addWidget(lbIcon);
    mLabels.append(lbIcon);
    QPixmap icon;
    if (!icon.load(iconPath))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
    }
    else
    {
        lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    QLabel* lbEngraveName = new QLabel(engrave);
    lbEngraveName->setFont(QFont("나눔스퀘어 네오 Bold", 10));
    lbEngraveName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    engraveLayout->addWidget(lbEngraveName);
    mLabels.append(lbEngraveName);

    QLabel* lbEngraveValue = new QLabel(QString("Lv. %1").arg(level));
    lbEngraveValue->setFont(QFont("나눔스퀘어 네오 Bold", 10));
    lbEngraveValue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    engraveLayout->addWidget(lbEngraveValue);
    mLabels.append(lbEngraveValue);

    if (engrave.contains("감소"))
    {
        lbEngraveName->setStyleSheet(QString("QLabel { color: red }"));
        lbEngraveValue->setStyleSheet(QString("QLabel { color: red }"));
    }

    return engraveLayout;
}
