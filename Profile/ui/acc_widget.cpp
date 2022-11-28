#include "acc_widget.h"
#include "ui_acc_widget.h"
#include "item/accessory.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFontDatabase>

AccWidget::AccWidget(QWidget *parent, const Accessory* acc, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::AccWidget),
    mAcc(acc),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

    requestIcon(iconUrl);
    setText();
    setStyleSheet();
}

AccWidget::~AccWidget()
{
    delete ui;
}

void AccWidget::requestIcon(QString iconUrl)
{
    QNetworkRequest request((QUrl(iconUrl + mAcc->getIconPath())));
    mNetworkManager->get(request);
}

void AccWidget::setText()
{
    Part part = mAcc->getPart();
    QString groupTitle;
    if (part == Part::NECK)
        groupTitle = "목걸이";
    else if (part == Part::EAR1 || part == Part::EAR2)
        groupTitle = "귀걸이";
    else if (part == Part::RING1 || part == Part::RING2)
        groupTitle = "반지";
    ui->groupAcc->setTitle(groupTitle);

    ui->barQual->setValue(mAcc->getQuality());
    ui->lbName->setText(mAcc->getName());
    ui->lbAbility->setText(mAcc->getAbility());

    QString engraveText;
    const QStringList& engraves = mAcc->getEngraves();
    for (const QString& engrave : engraves)
        engraveText.append(engrave);
    engraveText.append(mAcc->getPenalty());
    ui->lbEngrave->setText(engraveText);
}

void AccWidget::setStyleSheet()
{
    QString familyNanumRegular = QFontDatabase::applicationFontFamilies(1).at(0);
    QString familyNanumBold = QFontDatabase::applicationFontFamilies(2).at(0);
    QFont fontNanumRegular10 = QFont(familyNanumRegular, 10);
    QFont fontNanumBold10 = QFont(familyNanumBold, 10);

    ui->groupAcc->setFont(fontNanumRegular10);
    ui->barQual->setFont(fontNanumBold10);
    ui->lbAbility->setFont(fontNanumBold10);
    ui->lbEngrave->setFont(fontNanumBold10);
    ui->lbName->setFont(fontNanumBold10);

    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->barQual->setStyleSheet(QString("QProgressBar::chunk { background-color: %1 }").arg(getQualColor()));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(mAcc->getGrade())]));
}

QString AccWidget::getQualColor()
{
    QString color;
    int quality = mAcc->getQuality();

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

void AccWidget::slotUpdateIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
