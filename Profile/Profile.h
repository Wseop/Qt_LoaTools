#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QRegularExpression>

#include "character.h"

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

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

    void profileRequest(QString name);

private:
    void initUI();
    void initConnect();

    void parseTitle(const QString& profile);
    void parseCharacterList(const QString& profile);
    void parseGuildName(const QString& profile);

    void parseItem();
    void parseEquip(const QJsonObject& equipObj, Part part);
    void parseAccessory(const QJsonObject& accObj, Part part);
    void parseStone(const QJsonObject& stoneObj);
    void parseBracelet(const QJsonObject& braceletObj);
    void parseGem(const QJsonObject& gemObj);

    void parseEngrave();
    void parseCard();
    void parseSkill();

    Grade extractGrade(QString str);
    void extractEngraveValue(int type, QString engrave);

    Grade getGradeByColor(QString color);

    void clearAll();

private:
    Ui::Profile *ui;
    QNetworkAccessManager* mNetworkProfile = nullptr;
    QJsonObject* mProfile = nullptr;
    CharacterList* mCharacterList = nullptr;
    Character* mCharacter = nullptr;

    QRegularExpression mHtmlTag;

    // Item
    QList<class EquipWidget*> mEquipWidgets;
    QList<class AccWidget*> mAccWidgets;
    class AbilityStoneWidget* mStoneWidget = nullptr;
    class BraceletWidget* mBraceletWidget = nullptr;

    // Gem

    class EngraveWidget* mEngraveWidget = nullptr;
    QList<class CardLabel*> mCardLabels;

    // Skill

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
