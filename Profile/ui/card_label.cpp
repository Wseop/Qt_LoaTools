#include "card_label.h"
#include "ui_card_label.h"

CardLabel::CardLabel(QWidget *parent, QString cardTitle, QString cardDesc) :
    QWidget(parent),
    ui(new Ui::CardLabel)
{
    ui->setupUi(this);

    ui->lbCardTitle->setStyleSheet("QLabel { color: #00B700 }");

    ui->lbCardTitle->setText(cardTitle);
    ui->lbCardDesc->setText(cardDesc);
}

CardLabel::~CardLabel()
{
    delete ui;
}
