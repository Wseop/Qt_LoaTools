#include "gem_widget.h"
#include "ui_gem_widget.h"
#include "item/gem.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

GemWidget::GemWidget(QWidget *parent, const Gem* gem, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::GemWidget),
    mGem(gem),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

    requestIcon(iconUrl);
    setLevel();
    setName();
    setEffect();
}

GemWidget::~GemWidget()
{
    delete ui;
}

void GemWidget::requestIcon(QString iconUrl)
{
    QNetworkRequest request(QUrl(iconUrl + mGem->getIconPath()));
    mNetworkManager->get(request);
}

void GemWidget::setLevel()
{
    ui->lbLevel->setText(QString("Lv. %1").arg(mGem->GetLevel()));
    ui->lbLevel->setAlignment(Qt::AlignHCenter);
}

void GemWidget::setName()
{
    ui->lbName->setText(mGem->getName());
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(mGem->getGrade())]));
}

void GemWidget::setEffect()
{
    ui->lbEffect->setText(mGem->getEffect());
}

void GemWidget::slotUpdateIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
}
