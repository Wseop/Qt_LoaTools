#include "equip_widget.h"
#include "ui_equip_widget.h"
#include "item/equip.h"
#include "font/font_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

EquipWidget::EquipWidget(QWidget *parent, const Equip* equip, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::EquipWidget),
    mEquip(equip),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

    requestIcon(iconUrl);
    setText();
    setStyleSheet();
}

EquipWidget::~EquipWidget()
{
    delete ui;
}

void EquipWidget::requestIcon(QString iconUrl)
{
    QNetworkRequest request((QUrl(iconUrl + mEquip->getIconPath())));
    mNetworkManager->get(request);
}

void EquipWidget::setText()
{
    Part part = mEquip->getPart();
    QString groupTitle;
    if (part == Part::WEAPON)
        groupTitle = "무기";
    else if (part == Part::TOP)
        groupTitle = "상의";
    else if (part == Part::BOTTOM)
        groupTitle = "하의";
    else if (part == Part::HEAD)
        groupTitle = "머리";
    else if (part == Part::SHOULDER)
        groupTitle = "어깨";
    else if (part == Part::HAND)
        groupTitle = "장갑";
    ui->groupEquip->setTitle(groupTitle);

    ui->barQual->setValue(mEquip->getQuality());
    ui->lbName->setText(mEquip->getName());
    ui->lbLevelTier->setText(mEquip->getLevelTier());
    ui->lbSetLevel->setText(mEquip->getSetLevel());
}

void EquipWidget::setStyleSheet()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    ui->groupEquip->setFont(fontNanumRegular10);
    ui->lbLevelTier->setFont(fontNanumBold10);
    ui->lbName->setFont(fontNanumBold10);
    ui->lbSetLevel->setFont(fontNanumBold10);
    ui->barQual->setFont(fontNanumBold10);

    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->barQual->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualColor()));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(mEquip->getGrade())]));
}

QString EquipWidget::getQualColor()
{
    QString color;
    int quality = mEquip->getQuality();

    if (quality == 100)
        color = "#FE9600";
    else if (quality >= 90)
        color = "#CE43FC";
    else if (quality >= 70)
        color = "#00B5FF";
    else if (quality >= 30)
        color = "#00B700";
    else if (quality >= 10)
        color = "#FFD200";
    else
        color = "#FF6000";

    return color;
}

void EquipWidget::slotUpdateIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
