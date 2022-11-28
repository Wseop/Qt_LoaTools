#include "skill_widget.h"
#include "ui_skill_widget.h"
#include "profile/skill.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFontDatabase>

SkillWidget::SkillWidget(QWidget *parent, const Skill *skill, QString iconUrl) :
    QWidget(parent),
    ui(new Ui::SkillWidget),
    mSkill(skill),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);

    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotUpdateSkillIcon(QNetworkReply*)));

    ui->hLayoutSkill->setAlignment(Qt::AlignLeft);

    QFont fontNanumRegular10 = QFont(QFontDatabase::applicationFontFamilies(1).at(0), 10);
    ui->groupSkill->setFont(fontNanumRegular10);
    ui->groupRune->setFont(fontNanumRegular10);
    ui->groupTripod->setFont(fontNanumRegular10);

    requestSkillIcon(iconUrl);
    setSkillName();
    setTripods();
    setRuneIcon();
    setRuneName();
}

SkillWidget::~SkillWidget()
{
    for (QLabel* label : mTripods)
    {
        ui->vLayoutTripod->removeWidget(label);
        delete label;
    }
    mTripods.clear();

    delete ui;
}

void SkillWidget::requestSkillIcon(QString iconUrl)
{
    QNetworkRequest request(QUrl(iconUrl + mSkill->getIconPath()));
    mNetworkManager->get(request);
}

void SkillWidget::setSkillName()
{
    ui->lbSkillName->setFont(QFont(QFontDatabase::applicationFontFamilies(2).at(0), 10));
    ui->lbSkillName->setText(QString("%1 Lv.%2").arg(mSkill->getName()).arg(mSkill->getLevel()));
}

void SkillWidget::setTripods()
{
    const QList<Tripod>& tripods = mSkill->getTripods();

    for (int i = 0; i < tripods.size(); i++)
    {
        const Tripod& tripod = tripods[i];
        QLabel* lbTripod = new QLabel(QString("%1: %2 Lv.%3").arg(i + 1).arg(tripod.name).arg(tripod.level));
        lbTripod->setFont(QFont(QFontDatabase::applicationFontFamilies(2).at(0), 10));
        lbTripod->setStyleSheet(QString("QLabel { color: %1 }").arg(tripod.color));
        ui->vLayoutTripod->addWidget(lbTripod);
        mTripods.append(lbTripod);
    }
}

void SkillWidget::setRuneIcon()
{
    const Rune* rune = mSkill->getRune();
    if (rune == nullptr)
        return;

    QPixmap icon(rune->getIconPath());
    ui->lbRuneIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void SkillWidget::setRuneName()
{
    const Rune* rune = mSkill->getRune();
    if (rune == nullptr)
        return;

    ui->lbRuneName->setFont(QFont(QFontDatabase::applicationFontFamilies(2).at(0), 10));
    ui->lbRuneName->setText(rune->getName());
    ui->lbRuneName->setStyleSheet(QString("QLabel { color: %1 }").arg(gItemColor[static_cast<int>(rune->getGrade())]));
}

void SkillWidget::slotUpdateSkillIcon(QNetworkReply *reply)
{
    QPixmap icon;
    if (!icon.loadFromData(reply->readAll(), "PNG"))
    {
        qDebug() << Q_FUNC_INFO << "Icon load fail";
        return;
    }

    ui->lbSkillIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
