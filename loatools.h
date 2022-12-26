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
    class MeteorTimer* m_pMeteorTimer = nullptr;
    class AuctionCalc* m_pAuctionCalc = nullptr;
    class EngraveSimulator* m_pEngrave = nullptr;
    class CharacterSearch* m_pCharacterSearch = nullptr;
    class SettingAdviser* m_pSettingAdviser = nullptr;
    class SpecChecker* m_pSpecChecker = nullptr;

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
};
#endif // LOATOOLS_H
