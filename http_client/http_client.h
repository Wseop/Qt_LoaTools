#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpClient : public QObject
{
    Q_OBJECT
private:
    HttpClient();
    ~HttpClient();

    void loadConfig();

public:
    static HttpClient* getInstance();
    static void destroyInstance();

signals:
    void insertOrUpdateCharacter(QJsonDocument jsonDoc);
    void insertOrUpdateAbility(QJsonDocument jsonDoc);
    void insertOrUpdateEngrave(QJsonDocument jsonDoc);

private slots:
    void slotInsertOrUpdateCharacter(QJsonDocument jsonDoc);
    void slotInsertOrUpdateAbility(QJsonDocument jsonDoc);
    void slotInsertOrUpdateEngrave(QJsonDocument jsonDoc);

    void slotHandleReply(QNetworkReply* reply);

private:
    static HttpClient* m_pClient;

    QNetworkAccessManager m_networkCharacter;
    QNetworkAccessManager m_networkAbility;
    QNetworkAccessManager m_networkEngrave;

    QString m_serverUrl;
    QString m_insertKey;
    QString m_insertValue;
};

#endif // HTTPCLIENT_H
