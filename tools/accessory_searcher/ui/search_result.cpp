#include "search_result.h"
#include "ui_search_result.h"
#include "font/font_manager.h"
#include "game_data/item/enum/item_grade.h"
#include "tools/character_search/ui/quality_color.h"
#include "tools/accessory_searcher/accessory_searcher.h"

SearchResult::SearchResult() :
    ui(new Ui::SearchResult)
{
    ui->setupUi(this);
    ui->pbDelete->hide();

    setFonts();
    initConnects();
}

SearchResult::~SearchResult()
{
    delete ui;
}

void SearchResult::setItemName(QString itemName)
{
    ui->lbItemName->setText(itemName);
}

void SearchResult::setItemGrade(QString itemGrade)
{
    ui->lbItemGrade->setText(itemGrade);
    ui->lbItemGrade->setStyleSheet(QString("QLabel { color: %1 }").arg(colorCode(strToItemGrade(itemGrade))));
}

void SearchResult::setItemQuality(int itemQuality)
{
    ui->lbItemQuality->setText(QString::number(itemQuality));
    ui->lbItemQuality->setStyleSheet(QString("QLabel { color: %1 }").arg(getQualityColor(itemQuality)));
}

void SearchResult::setRemainTime(QString endDate)
{
    // TODO. endData - 현재 시간
    ui->lbRemainTime->setText(endDate);
}

void SearchResult::setBuyPrice(int buyPrice)
{
    ui->lbBuyPrice->setText(QString::number(buyPrice));
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

    ui->groupRemainTime->setFont(nanumRegular10);
    ui->groupBuyPrice->setFont(nanumRegular10);
}

void SearchResult::initConnects()
{
    connect(ui->pbPick, &QPushButton::pressed, this, [&](){
        ui->pbPick->hide();
        ui->pbDelete->show();
        AccessorySearcher::getInstance()->moveToPickedList(this);
    });
    connect(ui->pbDelete, &QPushButton::pressed, this, [&](){
        AccessorySearcher::getInstance()->deleteFromPickedList(this);
        delete this;
    });
}
