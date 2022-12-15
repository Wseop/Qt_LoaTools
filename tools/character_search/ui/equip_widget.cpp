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

EquipWidget::EquipWidget(QWidget *pParent, const Equip* pEquip) :
    ui(new Ui::EquipWidget),
    m_pParent(pParent),
    m_pEquip(pEquip),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupEquip->setTitle(itemTypeToStr(m_pEquip->getType()));

    loadIcon();
    setQualityBar();
    setLabels();
    setFonts();
    setLayoutAlignments();
}

EquipWidget::~EquipWidget()
{
    delete m_pNetworkManager;
    delete ui;
}

void EquipWidget::loadIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pEquip->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* pReply){
        QPixmap icon;
        if (icon.loadFromData(pReply->readAll(), "PNG"))
        {
            ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
        }
    });
    m_pNetworkManager->get(request);
}

void EquipWidget::setQualityBar()
{
    int quality = m_pEquip->getQuality();
    ui->pbarQuality->setValue(quality);
    ui->pbarQuality->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualityColor(quality)));
}

void EquipWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setText(m_pEquip->getName());
    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pEquip->getGrade())));

    ui->lbLevelTier->setText(m_pEquip->getLevelTier());

    SetEffect setEffect = m_pEquip->getSetEffect();
    if (setEffect == SetEffect::에스더 && m_pEquip->isElla())
    {
        ui->lbSetEffectElla->setText("엘라 부여");
        ui->lbSetEffectElla->setStyleSheet(labelColor.arg(colorCode(ItemGrade::에스더)));
    }
    else
    {
        QString setEffectText = QString("%1 Lv.%2").arg(setEffectToStr(setEffect)).arg(m_pEquip->getSetLevel());
        ui->lbSetEffectElla->setText(setEffectText);
        ui->lbSetEffectElla->setStyleSheet(labelColor.arg("#000000"));
    }
}

void EquipWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    ui->lbLevelTier->setFont(nanumBold10);
    ui->lbSetEffectElla->setFont(nanumBold10);
    ui->pbarQuality->setFont(nanumBold10);
    ui->groupEquip->setFont(nanumRegular10);
}

void EquipWidget::setLayoutAlignments()
{
    ui->vLayoutLeft->setAlignment(Qt::AlignVCenter);
    ui->vLayoutRight->setAlignment(Qt::AlignVCenter);
}
