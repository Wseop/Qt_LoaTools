#ifndef AUCTION_CALC_H
#define AUCTION_CALC_H

#include <QWidget>

namespace Ui {
class AuctionCalc;
}

class AuctionCalc : public QWidget
{
    Q_OBJECT

public:
    explicit AuctionCalc(QWidget *parent = nullptr);
    ~AuctionCalc();

public:
    static AuctionCalc* getInstance();
    static void destroyInstance();

private:
    void initUI();
    void initConnect();

private:
    const double DIVIDE_N4 = 0.71;
    const double DIVIDE_N8 = 0.83;
    const double BID_4 = 0.65;
    const double BID_8 = 0.76;

    Ui::AuctionCalc *ui;
    class QIntValidator* mIntValidator;

    static AuctionCalc* m_pAuctionCalc;

private slots:
    void slotCalc();
};

#endif // AUCTION_CALC_H
