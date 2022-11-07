#include "bracelet_widget.h"
#include "ui_bracelet_widget.h"
#include "item/bracelet.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

BraceletWidget::BraceletWidget(QWidget *parent, const Bracelet* bracelet, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::BraceletWidget),
    mBracelet(bracelet),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

    requestIcon(iconUrl);
    setText();
    setStyleSheet();
}

BraceletWidget::~BraceletWidget()
{
    delete ui;
}

void BraceletWidget::requestIcon(QString iconUrl)
{
    QNetworkRequest request((QUrl(iconUrl + mBracelet->getIconPath())));
    mNetworkManager->get(request);
}

void BraceletWidget::setText()
{
    ui->lbName->setText(mBracelet->getName());
    ui->lbEffect->setText(mBracelet->getEffect());
}

void BraceletWidget::setStyleSheet()
{
    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(mBracelet->getGrade())]));
}

void BraceletWidget::slotUpdateIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
