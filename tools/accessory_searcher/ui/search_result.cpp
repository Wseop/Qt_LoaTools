#include "search_result.h"
#include "ui_search_result.h"
#include "font/font_manager.h"
#include "game_data/item/enum/item_grade.h"
#include "game_data/engrave/engrave_manager.h"
#include "tools/character_search/ui/quality_color.h"
#include "tools/accessory_searcher/accessory_searcher.h"

#include <QDateTime>

const int DAY_MSEC = 86400000;

SearchResult::SearchResult() :
    ui(new Ui::SearchResult)
{
    ui->setupUi(this);
    ui->pbDelete->hide();

    setFonts();
    setAlignments();
    initConnects();
}

SearchResult::~SearchResult()
{
    for (QLabel* pLabel : m_labels)
        delete pLabel;
    delete ui;
}

void SearchResult::setItemName(const QString& itemName)
{
    ui->lbItemName->setText(itemName);
}

void SearchResult::setItemGrade(const QString& itemGrade)
{
    ui->lbItemGrade->setText(itemGrade);
    ui->lbItemGrade->setStyleSheet(QString("QLabel { color: %1 }").arg(colorCode(strToItemGrade(itemGrade))));
}

void SearchResult::setItemQuality(int itemQuality)
{
    ui->lbItemQuality->setText(QString::number(itemQuality));
    ui->lbItemQuality->setStyleSheet(QString("QLabel { color: %1 }").arg(getQualityColor(itemQuality)));
}

void SearchResult::setRemainTime(const QString& endDate)
{
    QDateTime endDateTime = QDateTime::fromString(endDate, Qt::ISODateWithMs);
    int remainMsec = QDateTime::currentDateTime().msecsTo(endDateTime);

    int remainDay = 0;
    if (remainMsec >= DAY_MSEC)
    {
        remainDay = remainMsec / DAY_MSEC;
        remainMsec -= remainDay * DAY_MSEC;
    }

    QTime remainTime = QTime::fromMSecsSinceStartOfDay(remainMsec);
    QString remainTimeText;
    if (remainDay == 0)
    {
        if (remainTime.hour() == 0)
        {
            remainTimeText = QString("%1분").arg(remainTime.minute());
            ui->lbRemainTime->setStyleSheet("QLabel { color: red }");
        }
        else
        {
            remainTimeText = QString("%1시간 %2분").arg(remainTime.hour()).arg(remainTime.minute());
        }
    }
    else
    {
        remainTimeText = QString("%1일 %2시간").arg(remainDay).arg(remainTime.hour());
    }

    ui->lbRemainTime->setText(remainTimeText);
}

void SearchResult::setBuyPrice(int buyPrice)
{
    ui->lbBuyPrice->setText(QString("%L1").arg(buyPrice));
}

void SearchResult::setAbility(const QMap<QString, int>& abilities)
{
    QString abilityText = "%1 +%2";

    for (auto iter = abilities.begin(); iter != abilities.end(); iter++)
    {
        QString ability = iter.key();
        int value = iter.value();
        QLabel* pLabel = createLabel(abilityText.arg(ability).arg(value));
        ui->vLayoutAbility->addWidget(pLabel);
    }
}

void SearchResult::setEngraving(const QMap<QString, int>& engravings)
{
    QString engravingText = "%1 +%2";

    for (auto iter = engravings.begin(); iter != engravings.end(); iter++)
    {
        QString engraving = iter.key();
        int value = iter.value();
        QLabel* pLabel = createLabel(engravingText.arg(engraving).arg(value));
        if (EngraveManager::getInstance()->isValidPenalty(engraving))
        {
            pLabel->setStyleSheet("QLabel { color: red }");
            ui->vLayoutPenalty->addWidget(pLabel);
        }
        else
        {
            ui->vLayoutEngraving->addWidget(pLabel);
        }
    }
}

void SearchResult::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbItemName->setFont(nanumBold10);
    ui->lbItemGrade->setFont(nanumBold10);
    ui->lbItemQuality->setFont(nanumBold10);
    ui->lbRemainTime->setFont(nanumBold10);
    ui->lbBuyPrice->setFont(nanumBold10);
    ui->pbPick->setFont(nanumBold10);
    ui->pbDelete->setFont(nanumBold10);

    ui->groupResult->setFont(nanumRegular10);
    ui->groupRemainTime->setFont(nanumRegular10);
    ui->groupBuyPrice->setFont(nanumRegular10);
}

void SearchResult::setAlignments()
{
    ui->hLayoutGroupResult->setAlignment(Qt::AlignHCenter);
}

void SearchResult::initConnects()
{
    connect(ui->pbPick, &QPushButton::pressed, this, [&](){
        ui->pbPick->hide();
        ui->pbDelete->show();
        ui->groupResult->setTitle("고정");
        AccessorySearcher::getInstance()->moveToPickedList(this);
    });
    connect(ui->pbDelete, &QPushButton::pressed, this, [&](){
        AccessorySearcher::getInstance()->deleteFromPickedList(this);
        delete this;
    });
}

QLabel* SearchResult::createLabel(QString text)
{
    QLabel* pLabel = new QLabel(text);
    pLabel->setFont(FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10));
    pLabel->setFixedWidth(125);
    m_labels.append(pLabel);
    return pLabel;
}
