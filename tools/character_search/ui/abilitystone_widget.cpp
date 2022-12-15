#include "abilitystone_widget.h"
#include "ui_abilitystone_widget.h"
#include "game_data/item/abilitystone.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

AbilityStoneWidget::AbilityStoneWidget(QWidget* pParent, const AbilityStone* pStone) :
    ui(new Ui::AbilityStoneWidget),
    m_pParent(pParent),
    m_pStone(pStone),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupAbilityStone->setTitle(itemTypeToStr(m_pStone->getType()));

    loadIcon();
    setLabels();
    setFonts();
    setLayoutAlignments();
}

AbilityStoneWidget::~AbilityStoneWidget()
{
    for (QLabel* label : m_engraveLabels)
        delete label;
    m_engraveLabels.clear();

    delete m_pNetworkManager;
    delete ui;
}

void AbilityStoneWidget::loadIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pStone->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* pReply){
        QPixmap icon;
        if (!icon.loadFromData(pReply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    });
    m_pNetworkManager->get(request);
}

void AbilityStoneWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setText(m_pStone->getName());
    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pStone->getGrade())));

    const auto& engraves = m_pStone->getEngraves();
    for (const QPair<QString, int>& engrave : engraves)
    {
        QString engraveText = QString("[%1] +%2").arg(engrave.first).arg(engrave.second);
        QLabel* pEngraveLabel = createEngraveLabel(engraveText, labelColor.arg("#F7B838"));
        ui->vLayoutRight->addWidget(pEngraveLabel);
    }

    const QPair<QString, int>& penalty = m_pStone->getPenalty();
    if (penalty.first != "")
    {
        QString engraveText = QString("[%1] +%2").arg(penalty.first).arg(penalty.second);
        QLabel* pEngraveLabel = createEngraveLabel(engraveText, labelColor.arg("red"));
        ui->vLayoutRight->addWidget(pEngraveLabel);
    }
}

QLabel* AbilityStoneWidget::createEngraveLabel(QString labelText, QString labelColor)
{
    QLabel* pEngraveLabel = new QLabel(labelText);
    pEngraveLabel->setStyleSheet(labelColor);
    m_engraveLabels.append(pEngraveLabel);
    return pEngraveLabel;
}

void AbilityStoneWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* pLabel : m_engraveLabels)
        pLabel->setFont(nanumBold10);
    ui->groupAbilityStone->setFont(nanumRegular10);
}

void AbilityStoneWidget::setLayoutAlignments()
{
    ui->vLayoutRight->setAlignment(Qt::AlignVCenter);
}
