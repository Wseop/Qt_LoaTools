#include "bracelet_widget.h"
#include "ui_bracelet_widget.h"
#include "game_data/item/bracelet.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

BraceletWidget::BraceletWidget(QWidget* pParent, const Bracelet* pBracelet) :
    ui(new Ui::BraceletWidget),
    m_pParent(pParent),
    m_pBracelet(pBracelet),
    m_pNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->groupBracelet->setTitle(itemTypeToStr(m_pBracelet->getType()));

    loadIcon();
    setLabels();
    setFonts();
    setLayoutAlignments();
}

BraceletWidget::~BraceletWidget()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    m_labels.clear();

    delete m_pNetworkManager;
    delete ui;
}

void BraceletWidget::loadIcon()
{
    QNetworkRequest request;
    request.setUrl(m_pBracelet->getIconPath());

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

void BraceletWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbName->setText(m_pBracelet->getName());
    ui->lbName->setStyleSheet(labelColor.arg(colorCode(m_pBracelet->getGrade())));

    const QStringList& effects = m_pBracelet->getEffects();
    for (const QString& effect : effects)
    {
        QLabel* pEffectLabel = createEffectLabels(effect);
        ui->vLayoutEffect->addWidget(pEffectLabel);
    }

    const QStringList& specialEffects = m_pBracelet->getSpecialEffects();
    for (const QString& effect : specialEffects)
    {
        QLabel* pEffectLabel = createEffectLabels(effect);
        ui->vLayoutEffect->addWidget(pEffectLabel);
    }
}

QLabel* BraceletWidget::createEffectLabels(QString labelText)
{
    QLabel* pLabel = new QLabel(labelText);
    m_labels.append(pLabel);
    return pLabel;
}

void BraceletWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    for (QLabel* pLabel : m_labels)
        pLabel->setFont(nanumBold10);
    ui->groupBracelet->setFont(nanumRegular10);
}

void BraceletWidget::setLayoutAlignments()
{
    ui->vLayoutEffect->setAlignment(Qt::AlignVCenter);
}
