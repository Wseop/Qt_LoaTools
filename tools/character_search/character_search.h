#ifndef CHARACTER_SEARCH_H
#define CHARACTER_SEARCH_H

#include <functional>

#include <QWidget>
#include <QRegularExpression>

class QNetworkReply;

namespace Ui {
class CharacterSearch;
}

class CharacterSearch : public QWidget
{
    Q_OBJECT

private:
    CharacterSearch();
    ~CharacterSearch();

    void initConnect();
    void initAlignment();
    void initStyleSheet();
    void initFont();
    void initNetworkManagerPool();
    void initNetworkReplyHandlers();

    void sendRequests();
    void updateStatus(uint8_t statusBit);

    void reset();

private:
    // QNetworkReply data handlers
    static void handleCharacters(QNetworkReply* reply);
    static void handleProfiles(QNetworkReply* reply);
    static void handleEquipments(QNetworkReply* reply);
    static void handleSkills(QNetworkReply* reply);
    static void handleEngraves(QNetworkReply* reply);
    static void handleCards(QNetworkReply* reply);
    static void handleGems(QNetworkReply* reply);

public:
    void changeCharacter(QString name);

public:
    static CharacterSearch* getInstance();
    static void destroyInstance();

private:
    Ui::CharacterSearch *ui;
    class Character* m_pCharacter;
    QList<std::function<void(QNetworkReply*)>> m_replyHandlers;

    const int NETWORK_POOL_COUNT = 7;
    QList<class QNetworkAccessManager*> m_networkManagers;

    const uint8_t REPLY_HANDLE_FINISHED = 0x7F;
    uint8_t m_replyHandleStatus;

    class Others* m_pOthers;
    class ProfileWidget* m_pProfileWidget;
    QList<class EquipWidget*> m_equipWidgets;
    QList<class AccessoryWidget*> m_accessoryWidgets;
    class AbilityStoneWidget* m_pStoneWidget;
    class BraceletWidget* m_pBraceletWidget;

    static CharacterSearch* m_pCharacterSearch;
    static QRegularExpression m_regExpHtmlTag;
};

#endif // CHARACTER_SEARCH_H
