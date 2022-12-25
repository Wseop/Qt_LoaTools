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

    void sendGetRequest(QNetworkAccessManager* networkManager, LostarkApi api, int keyIndex, const QStringList& params);

private:
    static HttpClient* m_pHttpClient;

    QStringList m_apis;
    QString m_apiKey;
    QStringList m_apiKeys;
    class DB* m_pDB;
};

#endif // HTTPCLIENT_H
