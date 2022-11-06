#include "abilitystone_widget.h"
#include "ui_abilitystone_widget.h"
#include "item/abilitystone.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

AbilityStoneWidget::AbilityStoneWidget(QWidget *parent, const AbilityStone* stone, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::AbilityStoneWidget),
    mStone(stone),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

    requestIcon(iconUrl);
    setText();
    setStyleSheet();
}

AbilityStoneWidget::~AbilityStoneWidget()
{
    delete ui;
}

void AbilityStoneWidget::requestIcon(QString iconUrl)
{
    QNetworkRequest request((QUrl(iconUrl + mStone->getIconPath())));
    mNetworkManager->get(request);
}

void AbilityStoneWidget::setText()
{
    ui->lbName->setText(mStone->getName());

    const QStringList& engraves = mStone->getEngraves();
    QString engraveText;
    for (const QString& engrave : engraves)
        engraveText.append(engrave);
    engraveText.append(mStone->getPenalty());
    ui->lbEngrave->setText(engraveText);
}

void AbilityStoneWidget::setStyleSheet()
{
    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(mStone->getGrade())]));
}

void AbilityStoneWidget::slotUpdateIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
