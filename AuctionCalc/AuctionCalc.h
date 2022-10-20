#ifndef AUCTIONCALC_H
#define AUCTIONCALC_H

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

private slots:
    void slotCalc();
};

#endif // AUCTIONCALC_H
