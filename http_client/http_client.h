#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class HttpClient : public QObject
{
    Q_OBJECT

private:
    HttpClient();
    ~HttpClient();

    void loadApiKey();

public:
    static HttpClient* getInstance();
    static void destroyInstance();

    void sendRequest(QNetworkAccessManager* networkManager, const QString& url);

private:
    static HttpClient* m_pHttpClient;

    QString m_apiKey;
};

#endif // HTTPCLIENT_H
