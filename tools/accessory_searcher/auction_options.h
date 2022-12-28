#ifndef AUCTIONOPTIONS_H
#define AUCTIONOPTIONS_H

#include <QObject>
#include <QJsonObject>

class QNetworkAccessManager;

class AuctionOptions : public QObject
{
    Q_OBJECT

private:
    AuctionOptions();
    ~AuctionOptions();

    void loadAuctionOptions();

public:
    QList<int> getItemGradeQualities();
    QList<int> getAbilityCodes();
    QMap<QString, int> getEngravingCodes();
    QList<int> getAccessoryCodes();

    static AuctionOptions* getInstance();
    static void destroyInstance();

private:
    QNetworkAccessManager* m_pNetworkManager;
    QJsonObject m_auctionOptions;

    static AuctionOptions* m_pInstance;
};

#endif // AUCTIONOPTIONS_H
