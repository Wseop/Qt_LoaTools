#include "card_label.h"
#include "ui_card_label.h"
#include "font/font_manager.h"

CardLabel::CardLabel(QWidget *parent, QString cardTitle, QString cardDesc) :
    QWidget(parent),
    ui(new Ui::CardLabel)
{
    ui->setupUi(this);

    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);

    ui->lbCardDesc->setFont(fontNanumBold12);
    ui->lbCardTitle->setFont(fontNanumBold12);

    ui->lbCardTitle->setStyleSheet("QLabel { color: #00B700 }");

    ui->lbCardTitle->setText(cardTitle);
    ui->lbCardDesc->setText(cardDesc);
}

CardLabel::~CardLabel()
{
    delete ui;
}
