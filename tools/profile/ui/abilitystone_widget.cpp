#include "abilitystone_widget.h"
#include "ui_abilitystone_widget.h"
#include "item/abilitystone.h"
#include "font/font_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

AbilityStoneWidget::AbilityStoneWidget(const AbilityStone* stone, QString iconUrl) :
    ui(new Ui::AbilityStoneWidget),
    m_pStone(stone),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateIcon(QNetworkReply*)));

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
    QNetworkRequest request((QUrl(iconUrl + m_pStone->getIconPath())));
    m_pNetworkManager->get(request);
}

void AbilityStoneWidget::setText()
{
    ui->lbName->setText(m_pStone->getName());

    const QStringList& engraves = m_pStone->getEngraves();
    QString engraveText;
    for (const QString& engrave : engraves)
        engraveText.append(engrave);
    engraveText.append(m_pStone->getPenalty());
    ui->lbEngrave->setText(engraveText);
}

void AbilityStoneWidget::setStyleSheet()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumRegular10 = fontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->groupStone->setFont(fontNanumRegular10);
    ui->lbEngrave->setFont(fontNanumBold10);
    ui->lbName->setFont(fontNanumBold10);

    ui->lbIcon->setStyleSheet(QString("QLabel { border: 1px solid black }"));
    ui->lbName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(m_pStone->getGrade())]));
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
