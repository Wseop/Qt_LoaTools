#ifndef PROFILE_H
#define PROFILE_H

#include "character.h"

#include <QWidget>
#include <QRegularExpression>

namespace Ui {
class Profile;
}

const QString PATH_PROFILE = "https://lostark.game.onstove.com/Profile/Character";
const QString PATH_CDN = "https://cdn-lostark.game.onstove.com";

class QLabel;
class QLayout;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QProgressBar;
class QJsonObject;

class QNetworkAccessManager;
class QNetworkReply;

class CharacterList;

class Profile : public QWidget
{
    Q_OBJECT

private:
    Profile();
    ~Profile();

public:
    void profileRequest(QString name);

public:
    static Profile* getInstance();
    static void destroyInstance();

private:
    void initUI();
    void initConnect();

    void parseTitle(const QString& profile);
    void parseCharacterList(const QString& profile);
    void parseGuildName(const QString& profile);

    void parseItem();
    void parseEquip(const QJsonObject& equipObj, ItemPart part);
    void parseAccessory(const QJsonObject& accObj, ItemPart part);
    void parseStone(const QJsonObject& stoneObj);
    void parseBracelet(const QJsonObject& braceletObj);
    void parseGem(const QJsonObject& gemObj);

    void parseEngrave();
    void parseCard();
    void parseSkill();

    void updateDB();

    ItemGrade extractGrade(QString str);
    void extractEngraveValue(int type, QString engrave);

    ItemGrade getGradeByColor(QString color);

    void clearAll();

private:
    Ui::Profile *ui;
    QNetworkAccessManager* m_pNetworkProfile = nullptr;
    QJsonObject* m_pProfileObj = nullptr;
    CharacterList* m_pCharacterList = nullptr;
    Character* m_pCharacter = nullptr;
    class DBRequest* m_pDBRequest;

    QRegularExpression m_htmlTag;

    QList<class EquipWidget*> m_equipWidgets;
    QList<class AccWidget*> m_accWidgets;
    class AbilityStoneWidget* m_pStoneWidget = nullptr;
    class BraceletWidget* m_pBraceletWidget = nullptr;
    QList<class GemWidget*> m_gemWidgets;
    class EngraveWidget* m_pEngraveWidget = nullptr;
    QList<class CardLabel*> m_cardLabels;
    QList<class SkillWidget*> m_skillWidgets;

    static Profile* m_pProfile;

signals:
    void sigUpdateTitle();
    void sigUpdateItem();
    void sigUpdateGem();
    void sigUpdateEngrave();
    void sigUpdateCard();
    void sigUpdateSkill();

private slots:
    void slotProfileRequest();
    void slotExtractProfile(QNetworkReply* reply);
    void slotShowCharacterList();

    void slotUpdateTitle();
    void slotUpdateItem();
    void slotUpdateGem();
    void slotUpdateEngrave();
    void slotUpdateCard();
    void slotUpdateSkill();
};

#endif // PROFILE_H
