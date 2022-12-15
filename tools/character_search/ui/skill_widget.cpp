#include "skill_widget.h"
#include "ui_skill_widget.h"
#include "game_data/skill/skill.h"
#include "font/font_manager.h"

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

SkillWidget::SkillWidget(QWidget* pParent, const Skill* pSkill) :
    ui(new Ui::SkillWidget),
    m_pParent(pParent),
    m_pSkill(pSkill)
{
    ui->setupUi(this);

    loadSkillIcon();
    setTexts();
    setTripods();
    setRune();
    setFonts();
}

SkillWidget::~SkillWidget()
{
    for (QNetworkAccessManager* pNetworkManager : m_networkManagers)
        delete pNetworkManager;
    m_networkManagers.clear();

    for (QLabel* pLabel : m_labels)
        delete pLabel;
    m_labels.clear();

    for (QHBoxLayout* pLayout : m_hLayouts)
        delete pLayout;
    m_hLayouts.clear();

    delete ui;
}

void SkillWidget::setTexts()
{
    ui->lbName->setText(m_pSkill->getName());
    ui->lbLevel->setText(QString("Lv. %1").arg(m_pSkill->getLevel()));
}

void SkillWidget::setTripods()
{
    const QList<Tripod>& tripods = m_pSkill->getTripods();
    for (const Tripod& tripod : tripods)
    {
        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_hLayouts.append(hLayout);
        ui->vLayoutTripod->addLayout(hLayout);

        QString colorCode = tripodColorCode(tripod.tier);
        QString labelColor = QString("QLabel { color: %1 }").arg(colorCode);

        QLabel* pLbTripodTier = new QLabel(QString("%1:").arg(tripod.tier + 1));
        pLbTripodTier->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pLbTripodTier->setStyleSheet(labelColor);
        m_labels.append(pLbTripodTier);
        hLayout->addWidget(pLbTripodTier);

        QLabel* pLbTripodName = new QLabel(tripod.name);
        pLbTripodName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pLbTripodName->setStyleSheet(labelColor);
        m_labels.append(pLbTripodName);
        hLayout->addWidget(pLbTripodName);

        QLabel* pLbTripodLevel = new QLabel(QString("Lv. %1").arg(tripod.level));
        pLbTripodLevel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pLbTripodLevel->setStyleSheet(labelColor);
        m_labels.append(pLbTripodLevel);
        hLayout->addWidget(pLbTripodLevel);
    }
}

void SkillWidget::setRune()
{
    const SkillRune* pRune = m_pSkill->getRune();
    if (pRune == nullptr)
        return;

    QLabel* pLbRuneIcon = new QLabel();
    pLbRuneIcon->setFixedSize(50, 50);
    m_labels.append(pLbRuneIcon);
    ui->hLayoutRune->addWidget(pLbRuneIcon);
    loadRuneIcon(pRune->getIconPath(), pLbRuneIcon);

    QLabel* pLbRuneName = new QLabel(pRune->getName());
    pLbRuneName->setStyleSheet(QString("QLabel { color: %1 }").arg(colorCode(pRune->getGrade())));
    m_labels.append(pLbRuneName);
    ui->hLayoutRune->addWidget(pLbRuneName);
}

void SkillWidget::loadSkillIcon()
{
    QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager();
    m_networkManagers.append(pNetworkManager);

    QNetworkRequest request;
    request.setUrl(QUrl(m_pSkill->getIconPath()));

    connect(pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* pReply){
        QPixmap icon;
        if (icon.loadFromData(pReply->readAll(), "PNG"))
        {
            ui->lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            ui->lbIcon->setStyleSheet("QLabel { border: 1px solid black }");
        }
    });
    pNetworkManager->get(request);
}

void SkillWidget::loadRuneIcon(QString iconPath, QLabel* pLbRuneIcon)
{
    QNetworkAccessManager* pNetworkManager = new QNetworkAccessManager();
    m_networkManagers.append(pNetworkManager);

    QNetworkRequest request;
    request.setUrl(QUrl(iconPath));

    connect(pNetworkManager, &QNetworkAccessManager::finished, this, [pLbRuneIcon](QNetworkReply* pReply){
        QPixmap icon;
        if (icon.loadFromData(pReply->readAll(), "PNG"))
        {
            pLbRuneIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            pLbRuneIcon->setStyleSheet("QLabel { border: 1px solid black }");
        }
    });
    pNetworkManager->get(request);
}


void SkillWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbName->setFont(nanumBold10);
    ui->lbLevel->setFont(nanumBold10);
    for (QLabel* pLabel : m_labels)
        pLabel->setFont(nanumBold10);
    ui->groupTripod->setFont(nanumRegular10);
    ui->groupRune->setFont(nanumRegular10);
}
