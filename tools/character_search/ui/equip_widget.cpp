#include "equip_widget.h"
#include "ui_equip_widget.h"
#include "game_data/item/equip.h"
#include "tools/character_search/ui/quality_color.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

EquipWidget::EquipWidget(QWidget *parent, const Equip* equip) :
    ui(new Ui::EquipWidget),
    m_pParent(parent),
    m_pEquip(equip),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupEquip->setTitle(itemTypeToStr(m_pEquip->getType()));

    setFonts();
    setAlignment();
    setStyleSheets();
    requestIcon();
    setTexts();
    setQuality();
}

EquipWidget::~EquipWidget()
{
    delete m_pNetworkManager;
    delete ui;
}

void EquipWidget::setFonts()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont nanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    ui->lbLevelTier->setFont(nanumBold10);
    ui->lbSetEffectElla->setFont(nanumBold10);
    ui->pbarQuality->setFont(nanumBold10);
    ui->groupEquip->setFont(nanumRegular10);
}

void EquipWidget::setAlignment()
{
    ui->vLayoutLeft->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->vLayoutRight->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void EquipWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pEquip->getGrade())));
    if (m_pEquip->isElla())
    {
        ui->lbSetEffectElla->setStyleSheet(labelColor.arg(colorCode(ItemGrade::에스더)));
    }
    else
    {
        ui->lbSetEffectElla->setStyleSheet(labelColor.arg("#000000"));
    }
}

void EquipWidget::requestIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pEquip->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply){
        QPixmap icon;
        if (!icon.loadFromData(reply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    });
    m_pNetworkManager->get(request);
}

void EquipWidget::setTexts()
{
    ui->lbName->setText(m_pEquip->getName());
    ui->lbLevelTier->setText(m_pEquip->getLevelTier());

    SetEffect setEffect = m_pEquip->getSetEffect();
    if (setEffect == SetEffect::에스더 && m_pEquip->isElla())
    {
        ui->lbSetEffectElla->setText("엘라 부여");
    }
    else
    {
        QString setEffectText = QString("%1 Lv.%2").arg(setEffectToStr(setEffect)).arg(m_pEquip->getSetLevel());
        ui->lbSetEffectElla->setText(setEffectText);
    }
}

void EquipWidget::setQuality()
{
    int quality = m_pEquip->getQuality();
    ui->pbarQuality->setValue(quality);
    ui->pbarQuality->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualityColor(quality)));
}
