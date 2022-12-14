#include "bracelet_widget.h"
#include "ui_bracelet_widget.h"
#include "game_data/item/bracelet.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

BraceletWidget::BraceletWidget(QWidget* parent, const Bracelet* bracelet) :
    ui(new Ui::BraceletWidget),
    m_pParent(parent),
    m_pBracelet(bracelet),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupBracelet->setTitle(itemTypeToStr(m_pBracelet->getType()));

    setTexts();
    requestIcon();
    addEffectLabels();
    setFonts();
    setAlignments();
    setStyleSheets();
}

BraceletWidget::~BraceletWidget()
{
    for (QLabel* label : m_effectLabels)
        delete label;
    m_effectLabels.clear();

    delete m_pNetworkManager;
    delete ui;
}

void BraceletWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* label : m_effectLabels)
        label->setFont(nanumBold10);
    ui->groupBracelet->setFont(nanumRegular10);
}

void BraceletWidget::setAlignments()
{
    ui->vLayoutEffect->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    for (QLabel* label : m_effectLabels)
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void BraceletWidget::setStyleSheets()
{
    QString labelColor = QString("QLabel { color: %1 }");
    QString labelBorder = QString("QLabel { border: %1 %2 %3 }");

    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pBracelet->getGrade())));
    ui->lbIcon->setStyleSheet(labelBorder.arg("1px", "solid", "black"));
}

void BraceletWidget::requestIcon()
{
    QNetworkRequest request;
    request.setUrl(m_pBracelet->getIconPath());

    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply){
        QPixmap icon;
        if (!icon.loadFromData(reply->readAll(), "PNG"))
            return;
        ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    });
    m_pNetworkManager->get(request);
}

void BraceletWidget::setTexts()
{
    ui->lbName->setText(m_pBracelet->getName());
}

void BraceletWidget::addEffectLabels()
{
    const QStringList& effects = m_pBracelet->getEffects();

    for (const QString& effect : effects)
    {
        QLabel* label = new QLabel(effect);
        label->setFixedWidth(200);
        m_effectLabels.append(label);
        ui->vLayoutEffect->addWidget(label);
    }
}
