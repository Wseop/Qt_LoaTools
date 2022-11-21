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

public:
    const QNetworkAccessManager& getNetworkManagerCharacter();
    const QNetworkAccessManager& getNetworkManagerSetting();

signals:
    void insertOrUpdateCharacter(QJsonDocument jsonDoc);
    void insertOrUpdateSetting(QJsonDocument jsonDoc);

    void readCharacters();
    void readCharactersByClass(QString cls);
    void readCharacterByName(QString name);
    void readSettings();
    void readSettingsByClass(QString cls);
    void readSettingByName(QString name);

private slots:
    void slotInsertOrUpdateCharacter(QJsonDocument jsonDoc);
    void slotInsertOrUpdateSetting(QJsonDocument jsonDoc);
    void slotHandleReplyPost(QNetworkReply* reply);

    void slotReadCharacters();
    void slotReadCharactersByClass(QString cls);
    void slotReadCharacterByName(QString name);
    void slotReadSettings();
    void slotReadSettingsByClass(QString cls);
    void slotReadSettingByName(QString name);

private:
    static HttpClient* m_pClient;

    QNetworkAccessManager m_postManagerCharacter;
    QNetworkAccessManager m_postManagerSetting;
    QNetworkAccessManager m_getManagerCharacter;
    QNetworkAccessManager m_getManagerSetting;

    QString m_serverUrl;
    QString m_insertKey;
    QString m_insertValue;
};

#endif // HTTPCLIENT_H
