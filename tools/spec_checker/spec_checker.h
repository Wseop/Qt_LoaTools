#ifndef SPEC_CHECKER_H
#define SPEC_CHECKER_H

#include <functional>

#include <QWidget>
#include <QRegularExpression>

class QNetworkAccessManager;
class QNetworkReply;
class SpecWidget;
class Character;

namespace Ui {
class SpecChecker;
}

class SpecChecker : public QWidget
{
    Q_OBJECT

private:
    SpecChecker();
    ~SpecChecker();

    void initNetworkManagers();
    void initReplyHandlers();
    void initConnect();
    void initAlignments();
    void initFonts();

    void sendRequests();
    void updateStatus(uint8_t bit);
    void clearContents();
    void enableInput(bool enable);

    static void handleProfiles(QNetworkReply* pReply);
    static void handleEquipment(QNetworkReply* pReply);
    static void handleSkills(QNetworkReply* pReply);
    static void handleEngravings(QNetworkReply* pReply);
    static void handleCards(QNetworkReply* pReply);
    static void handleGems(QNetworkReply* pReply);

public:
    static SpecChecker* getInstance();
    static void destroyInstance();

private:
    Ui::SpecChecker *ui;

    const int MAX_NETWORK_MANAGER = 6;
    QList<QNetworkAccessManager*> m_networkManagers;

    const uint8_t REPLY_HANDLE_FINISHED = 0x3F;
    uint8_t m_replyHandleStatus;
    QList<std::function<void(QNetworkReply*)>> m_replyHandlers;

    Character* m_pNewCharacter;
    QList<SpecWidget*> m_specWidgets;

    static SpecChecker* m_pSpecChecker;
    static QRegularExpression m_regExpHtmlTag;
};

#endif // SPEC_CHECKER_H
