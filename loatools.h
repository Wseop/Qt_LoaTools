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
    class MeteorTimer* m_meteorTimer = nullptr;
    class AuctionCalc* m_auctionCalc = nullptr;
    class EngraveSimulator* m_engrave = nullptr;
    class Profile* m_profile = nullptr;
    class SettingAdviser* m_settingAdviser = nullptr;

private:
    void initConnect();
    void initFont();
    void initAlignment();

private slots:
    void slotOpenMeteorTimer();
    void slotOpenAuctionCalc();
    void slotOpenEngraveSimulator();
    void slotOpenProfile();
    void slotOpenSettingAdviser();
    void slotOpenRankingBoard();
};
#endif // LOATOOLS_H
