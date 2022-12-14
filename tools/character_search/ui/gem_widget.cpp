#include "gem_widget.h"
#include "ui_gem_widget.h"
#include "game_data/item/gem.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

GemWidget::GemWidget(QWidget* parent, const Gem* gem) :
    ui(new Ui::GemWidget),
    m_pParent(parent),
    m_pGem(gem),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupGem->setTitle(gemTypeToStr(m_pGem->getGemType()));

    setTexts();
    requestIcon();
    setFonts();
    setAlignments();
    setStyleSheets();
}

GemWidget::~GemWidget()
{
    delete m_pNetworkManager;
    delete ui;
}

void GemWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    ui->lbEffect->setFont(nanumBold10);
    ui->groupGem->setFont(nanumRegular10);
}

void GemWidget::setAlignments()
{
    ui->lbName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->lbEffect->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void GemWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");
    QString labelBorder = QString("QLabel { border: %1 %2 %3 }");

    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pGem->getGrade())));
    ui->lbIcon->setStyleSheet(labelBorder.arg("1px", "solid", "black"));
}

void GemWidget::setTexts()
{
    ui->lbName->setText(m_pGem->getName());
    ui->lbEffect->setText(m_pGem->getEffect());
}

void GemWidget::requestIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pGem->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply){
        QPixmap icon;
        if (!icon.loadFromData(reply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    });
    m_pNetworkManager->get(request);
}
