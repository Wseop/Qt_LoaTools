#include "accessory_widget.h"
#include "ui_accessory_widget.h"
#include "game_data/item/accessory.h"
#include "tools/character_search/ui/quality_color.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

AccessoryWidget::AccessoryWidget(QWidget* pParent, const Accessory* pAccessory) :
    ui(new Ui::AccessoryWidget),
    m_pParent(pParent),
    m_pAccessory(pAccessory),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupAccessory->setTitle(itemTypeToStr(m_pAccessory->getType()));

    loadIcon();
    setQualityBar();
    setLabels();
    setFonts();
    setLayoutAlignments();
}

AccessoryWidget::~AccessoryWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    delete m_pNetworkManager;
    delete ui;
}

void AccessoryWidget::loadIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pAccessory->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, [&](QNetworkReply* pReply){
        QPixmap icon;
        if (!icon.loadFromData(pReply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    });
    m_pNetworkManager->get(request);
}

void AccessoryWidget::setQualityBar()
{
    int quality = m_pAccessory->getQuality();
    ui->pbarQuality->setValue(quality);
    ui->pbarQuality->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualityColor(quality)));
}

void AccessoryWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setText(m_pAccessory->getName());
    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pAccessory->getGrade())));

    const auto& abilities = m_pAccessory->getAbilities();
    for (const QPair<Ability, int>& ability : abilities)
    {
        QString labelText = QString("%1 +%2").arg(abilityToStr(ability.first)).arg(ability.second);
        QLabel* pAbilityLabel = createLabel(labelText, labelColor.arg("#000000"));
        ui->hLayoutAbility->addWidget(pAbilityLabel);
    }

    const auto& engraves = m_pAccessory->getEngraves();
    for (const QPair<QString, int>& engrave : engraves)
    {
        QString labelText = QString("[%1] +%2").arg(engrave.first).arg(engrave.second);
        QLabel* pEngraveLabel = createLabel(labelText, labelColor.arg("#F7B838"));
        ui->vLayoutRight->addWidget(pEngraveLabel);
    }

    const QPair<QString, int>& penalty = m_pAccessory->getPenalty();
    if (penalty.first != "")
    {
        QString labelText = QString("[%1] +%2").arg(penalty.first).arg(penalty.second);
        QLabel* pPenaltyLabel = createLabel(labelText, labelColor.arg("red"));
        ui->vLayoutRight->addWidget(pPenaltyLabel);
    }
}

QLabel* AccessoryWidget::createLabel(QString labelText, QString labelColor)
{
    QLabel* pLabel = new QLabel(labelText);
    pLabel->setStyleSheet(labelColor);
    m_labels.append(pLabel);
    return pLabel;
}

void AccessoryWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* pLabel : m_labels)
        pLabel->setFont(nanumBold10);
    ui->pbarQuality->setFont(nanumBold10);
    ui->groupAccessory->setFont(nanumRegular10);
}

void AccessoryWidget::setLayoutAlignments()
{
    ui->vLayoutLeft->setAlignment(Qt::AlignVCenter);
    ui->vLayoutRight->setAlignment(Qt::AlignVCenter);
}
