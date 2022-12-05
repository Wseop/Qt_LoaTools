#include "gem_widget.h"
#include "ui_gem_widget.h"
#include "item/gem.h"
#include "font/font_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

GemWidget::GemWidget(const Gem* gem, QString iconUrl) :
    ui(new Ui::GemWidget),
    m_pGem(gem),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

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
    QNetworkRequest request(QUrl(iconUrl + m_pGem->getIconPath()));
    m_pNetworkManager->get(request);
}

void GemWidget::setLevel()
{
    QFont fontNanumBold10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10);
    ui->lbLevel->setFont(fontNanumBold10);
    ui->lbLevel->setText(QString("Lv. %1").arg(m_pGem->GetLevel()));
    ui->lbLevel->setAlignment(Qt::AlignHCenter);
}

void GemWidget::setName()
{
    QFont fontNanumBold10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10);
    ui->lbName->setFont(fontNanumBold10);
    ui->lbName->setText(m_pGem->getName());
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(m_pGem->getGrade())]));
}

void GemWidget::setEffect()
{
    QFont fontNanumBold10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10);
    ui->lbEffect->setFont(fontNanumBold10);
    ui->lbEffect->setText(m_pGem->getEffect());
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
