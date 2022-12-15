#include "gem_widget.h"
#include "ui_gem_widget.h"
#include "game_data/item/gem.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

GemWidget::GemWidget(QWidget* pParent, const Gem* pGem) :
    ui(new Ui::GemWidget),
    m_pParent(pParent),
    m_pGem(pGem),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupGem->setTitle(gemTypeToStr(m_pGem->getGemType()));

    loadIcon();
    setLabels();
    setFonts();
    setLayoutAlignments();
}

GemWidget::~GemWidget()
{
    delete m_pNetworkManager;
    delete ui;
}

void GemWidget::loadIcon()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_pGem->getIconPath()));

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* pReply){
        QPixmap icon;
        if (icon.loadFromData(pReply->readAll(), "PNG"))
        {
            ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            ui->lbIcon->setStyleSheet("QLabel { border: 1px solid black }");
        }
    });
    m_pNetworkManager->get(request);
}

void GemWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setText(m_pGem->getName());
    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pGem->getGrade())));

    ui->lbEffect->setText(m_pGem->getEffect());
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

void GemWidget::setLayoutAlignments()
{
    ui->vLayoutRight->setAlignment(Qt::AlignVCenter);
}
