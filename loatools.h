#ifndef LOATOOLS_H
#define LOATOOLS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoaTools; }
QT_END_NAMESPACE

class LoaTools : public QWidget
{
    Q_OBJECT

public:
    LoaTools(QWidget *parent = nullptr);
    ~LoaTools();

private:
    Ui::LoaTools *ui;
    class MeteorTimer* mMeteorTimer = nullptr;
    class AuctionCalc* mAuctionCalc = nullptr;
    class EngraveSimulator* mEngrave = nullptr;

private:
    void initConnect();

private slots:
    void slotOpenMeteorTimer();
    void slotOpenAuctionCalc();
    void slotOpenEngraveSimulator();
};
#endif // LOATOOLS_H
