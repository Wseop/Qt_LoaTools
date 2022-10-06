#include "loatools.h"
#include "ui_loatools.h"

LoaTools::LoaTools(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoaTools)
{
    ui->setupUi(this);
}

LoaTools::~LoaTools()
{
    delete ui;
}

