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
    void loadApiKey();

public:
    static HttpClient* getInstance();
    static void destroyInstance();

    void sendGetRequest(QNetworkAccessManager* networkManager, LostarkApi api, const QStringList& params);

private:
    static HttpClient* m_pHttpClient;

    QStringList m_apis;
    QString m_apiKey;
};

#endif // HTTPCLIENT_H
