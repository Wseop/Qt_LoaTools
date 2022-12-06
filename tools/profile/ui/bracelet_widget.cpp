#include "bracelet_widget.h"
#include "ui_bracelet_widget.h"
#include "item/bracelet.h"
#include "font/font_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

BraceletWidget::BraceletWidget(const Bracelet* bracelet, QString iconUrl) :
    ui(new Ui::BraceletWidget),
    m_pBracelet(bracelet),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

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
    QNetworkRequest request((QUrl(iconUrl + m_pBracelet->getIconPath())));
    m_pNetworkManager->get(request);
}

void BraceletWidget::setText()
{
    ui->lbName->setText(m_pBracelet->getName());
    ui->lbEffect->setText(m_pBracelet->getEffect());
}

void BraceletWidget::setStyleSheet()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->groupBracelet->setFont(fontNanumRegular10);
    ui->lbIcon->setFont(fontNanumBold10);
    ui->lbName->setFont(fontNanumBold10);
    ui->lbEffect->setFont(fontNanumBold10);

    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(m_pBracelet->getGrade())]));
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
