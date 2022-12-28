#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

enum class LostarkApi
{
    Characters,
    Profiles,
    Equipments,
    Skills,
    Engraves,
    Cards,
    Gems,
    AuctionOptions,
    Auction,
};

class HttpClient : public QObject
{
    Q_OBJECT

private:
    HttpClient();
    ~HttpClient();

    void loadApi();
    void loadApiKeys();

public:
    static HttpClient* getInstance();
    static void destroyInstance();

    void sendGetRequest(QNetworkAccessManager* pNetworkManager, LostarkApi api, int keyIndex, const QStringList& params);
    void sendPostRequest(QNetworkAccessManager* pNetworkManager, LostarkApi api, int keyIndex, QByteArray data);

private:
    static HttpClient* m_pHttpClient;

    const int MAX_APIKEY = 5;
    QStringList m_apis;
    QStringList m_apiKeys;
    class DB* m_pDB;
};

#endif // HTTPCLIENT_H
