#include "auction_calc.h"
#include "ui_auctioncalc.h"

#include <QIntValidator>

AuctionCalc::AuctionCalc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuctionCalc),
    mIntValidator(new QIntValidator())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("경매 계산기");

    initUI();
    initConnect();
}

AuctionCalc::~AuctionCalc()
{
    delete ui;
}

void AuctionCalc::initUI()
{
    ui->groupBoxPrice->setFixedHeight(45);
    ui->groupBoxDesc->setFixedHeight(45);

    // 배경색, 글자색 설정
    ui->lbDivideN4->setStyleSheet("QLabel { background-color : white }");
    ui->lbDivideN8->setStyleSheet("QLabel { background-color : white }");
    ui->lbBid4->setStyleSheet("QLabel { background-color : white; color : red }");
    ui->lbBid8->setStyleSheet("QLabel { background-color : white; color : red }");

    // border 설정
    ui->lbP4->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbP8->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbDivideN->setStyleSheet("QLabel { border : 1px solid black }");
    ui->lbBid->setStyleSheet("QLabel { border : 1px solid black }");

    // 숫자만 입력 가능하도록 설정
    ui->lePrice->setValidator(mIntValidator);
}

void AuctionCalc::initConnect()
{
    // EnterKey 연결
    connect(ui->lePrice, SIGNAL(returnPressed()), this, SLOT(slotCalc()));
    connect(ui->pbCalc, SIGNAL(pressed()), this, SLOT(slotCalc()));
}

void AuctionCalc::slotCalc()
{
    int divideN4, divideN8, bid4, bid8;
    double price;

    price = ui->lePrice->text().toDouble();
    // 최적 입찰가 계산
    divideN4 = price * DIVIDE_N4;
    divideN8 = price * DIVIDE_N8;
    bid4 = price * BID_4;
    bid8 = price * BID_8;

    // ui출력
    // %L~ : 1000단위마다 ,찍기
    ui->lbDivideN4->setText(QString("%L1").arg(divideN4));
    ui->lbDivideN8->setText(QString("%L1").arg(divideN8));
    ui->lbBid4->setText(QString("%L1").arg(bid4));
    ui->lbBid8->setText(QString("%L1").arg(bid8));
}
