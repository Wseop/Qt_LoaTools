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

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

    void profileRequest(QString name);

private:
    void initEngraveList();
    void initMap();
    void initUI();
    void initConnect();

    QVariant getValueFromJson(const QJsonObject& src, QStringList keys);

    void parseTitle(QString& profile);
    void parseCharacterList(QString& profile);
    void parseGuildName(QString& profile);
    void parseEquip();
    void parseGem();
    void parseEngrave();
    void parseSkill();
    void parseCard();

    void extractEngraveValue(QString engrave);

    void updateTitle();
    void updateEquip();
    void updateGem();
    void updateEngrave();
    void updateSkill();
    void updateCard();

    void requestIcon(class QNetworkAccessManager* networkManager, QString iconPath);
    void setQualityColor(Part part, int quality);
    void setNameColor(QLabel* label, Grade grade);
    Grade getItemGrade(const QJsonObject& obj);
    Grade getGradeByColor(QString color);
    QString getColorByGrade(Grade grade);

    void clearAll();

private:
    Ui::Profile *ui;
    class QNetworkAccessManager* mNetworkProfile = nullptr;
    class QNetworkAccessManager* mNetworkIconEquip = nullptr;
    class QNetworkAccessManager* mNetworkIconGem = nullptr;
    class QNetworkAccessManager* mNetworkIconSkill = nullptr;
    class QJsonObject* mProfile = nullptr;
    class CharacterList* mCharacterList = nullptr;
    Character* mCharacter = nullptr;

    QRegularExpression mHtmlTag;

    // Part Map
    QMap<Part, QLabel*> mPartIcon;
    QMap<QString, QList<Part>> mPathParts;
    QMap<Part, class QProgressBar*> mPartQual;
    QMap<Part, QLabel*> mPartName;
    QMap<Part, QLabel*> mPartLevel;
    QMap<Part, QLabel*> mPartSet;
    QMap<Part, QLabel*> mPartAttr;
    QMap<Part, QLabel*> mPartEngrave;

    // Gem Map
    QList<QLabel*> mGemIcons;
    QMap<QString, QList<int>> mGemPathIndex;
    QList<QLabel*> mGemLevels;
    QList<QLabel*> mGemNames;
    QList<QLabel*> mGemAttrs;

    // Engrave
    QList<QLabel*> mEngraveLabels;
    QList<QHBoxLayout*> mEngraveLayouts;
    QList<QHBoxLayout*> mPenaltyLayouts;

    // Card
    QList<QVBoxLayout*> mCardLayoutList;
    QList<QLabel*> mCardLabelList;

    // Skill
    QList<QGroupBox*> mSkillGroupBoxes;
    QList<QLayout*> mSkillLayouts;
    QList<QLabel*> mSkillLabels;
    QMap<QString, QLabel*> mSkillIconLabel;

private slots:
    void slotProfileRequest();
    void slotExtractProfile(class QNetworkReply* reply);
    void slotSetIconEquip(class QNetworkReply* reply);
    void slotSetIconGem(class QNetworkReply* reply);
    void slotSetIconSkill(class QNetworkReply* reply);
    void slotShowCharacterList();
};

#endif // PROFILE_H
