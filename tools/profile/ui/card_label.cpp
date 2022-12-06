#include "card_label.h"
#include "ui_card_label.h"
#include "font/font_manager.h"

CardLabel::CardLabel(QString cardTitle, QString cardDesc) :
    ui(new Ui::CardLabel)
{
    ui->setupUi(this);

    initFont();

    ui->lbCardTitle->setStyleSheet("QLabel { color: #00B700 }");
    ui->lbCardTitle->setText(cardTitle);
    ui->lbCardDesc->setText(cardDesc);
}

CardLabel::~CardLabel()
{
    delete ui;
}

void CardLabel::initFont()
{
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);

    ui->lbCardDesc->setFont(fontNanumBold12);
    ui->lbCardTitle->setFont(fontNanumBold12);
}
