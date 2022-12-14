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

AccessoryWidget::AccessoryWidget(QWidget* parent, const Accessory* accessory) :
    ui(new Ui::AccessoryWidget),
    m_pParent(parent),
    m_pAccessory(accessory),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupAccessory->setTitle(itemTypeToStr(m_pAccessory->getType()));

    setAlignment();
    requestIcon();
    setTexts();
    setQuality();
    addAbilities();
    addEngraves();
    setFonts();
}

AccessoryWidget::~AccessoryWidget()
{
    for (QLabel* label : m_abilityLabels)
        delete label;
    m_abilityLabels.clear();

    for (QLabel* label : m_engraveLabels)
        delete label;
    m_engraveLabels.clear();

    delete m_pNetworkManager;
    delete ui;
}

void AccessoryWidget::setFonts()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont nanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* label : m_abilityLabels)
        label->setFont(nanumBold10);
    for (QLabel* label : m_engraveLabels)
        label->setFont(nanumBold10);
    ui->pbarQuality->setFont(nanumBold10);
    ui->groupAccessory->setFont(nanumRegular10);
}

void AccessoryWidget::setAlignment()
{
    ui->vLayoutLeft->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->vLayoutRight->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->hLayoutAbility->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void AccessoryWidget::requestIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pAccessory->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, [&](QNetworkReply* reply){
        QPixmap icon;
        if (!icon.loadFromData(reply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    });
    m_pNetworkManager->get(request);
}

void AccessoryWidget::setTexts()
{
    ui->lbName->setText(m_pAccessory->getName());
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(colorCode(m_pAccessory->getGrade())));
}

void AccessoryWidget::setQuality()
{
    int quality = m_pAccessory->getQuality();
    ui->pbarQuality->setValue(quality);
    ui->pbarQuality->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualityColor(quality)));
}

void AccessoryWidget::addAbilities()
{
    const auto& abilities = m_pAccessory->getAbilities();
    for (const QPair<Ability, int>& ability : abilities)
    {
        QString labelText = QString("%1 +%2").arg(abilityToStr(ability.first)).arg(ability.second);
        QLabel* abilityLabel = new QLabel(labelText);
        abilityLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        abilityLabel->setFixedSize(70, 25);
        m_abilityLabels.append(abilityLabel);
        ui->hLayoutAbility->addWidget(abilityLabel);
    }
}

void AccessoryWidget::addEngraves()
{
    const auto& engraves = m_pAccessory->getEngraves();
    for (const QPair<QString, int>& engrave : engraves)
    {
        QString labelText = QString("[%1] +%2").arg(engrave.first).arg(engrave.second);
        QLabel* engraveLabel = new QLabel(labelText);
        engraveLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        engraveLabel->setFixedSize(150, 25);
        engraveLabel->setStyleSheet("QLabel { color: #F7B838 }");
        m_engraveLabels.append(engraveLabel);
        ui->vLayoutRight->addWidget(engraveLabel);
    }

    const QPair<QString, int>& penalty = m_pAccessory->getPenalty();
    if (penalty.first != "")
    {
        QString labelText = QString("[%1] +%2").arg(penalty.first).arg(penalty.second);
        QLabel* penaltyLabel = new QLabel(labelText);
        penaltyLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        penaltyLabel->setFixedSize(150, 25);
        penaltyLabel->setStyleSheet("QLabel { color: red }");
        m_engraveLabels.append(penaltyLabel);
        ui->vLayoutRight->addWidget(penaltyLabel);
    }
}
