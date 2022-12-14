#include "abilitystone_widget.h"
#include "ui_abilitystone_widget.h"
#include "game_data/item/abilitystone.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

AbilityStoneWidget::AbilityStoneWidget(QWidget* parent, const AbilityStone* stone) :
    ui(new Ui::AbilityStoneWidget),
    m_pParent(parent),
    m_pStone(stone),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupAbilityStone->setTitle(itemTypeToStr(m_pStone->getType()));

    requestIcon();
    setTexts();
    addEngraveLabels();
    setFonts();
    setStyleSheets();
}

AbilityStoneWidget::~AbilityStoneWidget()
{
    for (QLabel* label : m_engraveLabels)
        delete label;
    m_engraveLabels.clear();

    delete ui;
}

void AbilityStoneWidget::setFonts()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont nanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* label : m_engraveLabels)
        label->setFont(nanumBold10);
    ui->groupAbilityStone->setFont(nanumRegular10);
}

void AbilityStoneWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pStone->getGrade())));
}

void AbilityStoneWidget::requestIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pStone->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply){
        QPixmap icon;
        if (!icon.loadFromData(reply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    });
    m_pNetworkManager->get(request);
}

void AbilityStoneWidget::setTexts()
{
    ui->lbName->setText(m_pStone->getName());
}

void AbilityStoneWidget::addEngraveLabels()
{
    const auto& engraves = m_pStone->getEngraves();
    for (const QPair<QString, int>& engrave : engraves)
    {
        QString labelText = QString("[%1] +%2").arg(engrave.first).arg(engrave.second);
        QLabel* label = new QLabel(labelText);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setFixedSize(150, 25);
        label->setStyleSheet("QLabel { color: #F7B838 }");
        m_engraveLabels.append(label);
        ui->vLayoutRight->addWidget(label);
    }

    const QPair<QString, int>& penalty = m_pStone->getPenalty();
    if (penalty.first != "")
    {
        QString labelText = QString("[%1] +%2").arg(penalty.first).arg(penalty.second);
        QLabel* label = new QLabel(labelText);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setFixedSize(150, 25);
        label->setStyleSheet("QLabel { color: red }");
        m_engraveLabels.append(label);
        ui->vLayoutRight->addWidget(label);
    }
}
