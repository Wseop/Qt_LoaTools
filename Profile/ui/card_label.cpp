#include "card_label.h"
#include "ui_card_label.h"

#include <QFontDatabase>

CardLabel::CardLabel(QWidget *parent, QString cardTitle, QString cardDesc) :
    QWidget(parent),
    ui(new Ui::CardLabel)
{
    ui->setupUi(this);

    QFont fontNanumBold12 = QFont(QFontDatabase::applicationFontFamilies(2).at(0), 12);

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
