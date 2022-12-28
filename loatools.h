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

private:
    void initConnect();
    void initFont();
    void initAlignment();

private slots:
    void slotOpenMeteorTimer();
    void slotOpenAuctionCalc();
    void slotOpenEngraveSimulator();
    void slotOpenCharacterSearch();
    void slotOpenSettingAdviser();
    void slotOpenRankingBoard();
    void slotOpenSpecChecker();
    void slotOpenAccessorySearcher();
};
#endif // LOATOOLS_H
