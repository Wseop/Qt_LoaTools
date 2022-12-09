#ifndef CHARACTER_SEARCH_H
#define CHARACTER_SEARCH_H

#include <functional>

#include <QWidget>
#include <QRegularExpression>

class Character;
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
    void initHandlers();

    void sendRequests();

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
    static CharacterSearch* getInstance();
    static void destroyInstance();

private:
    Ui::CharacterSearch *ui;
    Character* m_pCharacter;
    QList<std::function<void(QNetworkReply*)>> m_handlers;

    static CharacterSearch* m_pCharacterSearch;
    static QRegularExpression m_regExpHtmlTag;
};

#endif // CHARACTER_SEARCH_H
