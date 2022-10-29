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
    void setNameColor(class QLabel* label, Grade grade);
    Grade getItemGrade(const QJsonObject& obj);

    void clearAll();

private:
    Ui::Profile *ui;
    class QNetworkAccessManager* mNetworkProfile = nullptr;
    class QNetworkAccessManager* mNetworkIconEquip = nullptr;
    class QNetworkAccessManager* mNetworkIconGem = nullptr;
    class QJsonObject* mProfile = nullptr;
    class CharacterList* mCharacterList = nullptr;
    Character* mCharacter = nullptr;

    QRegularExpression mHtmlTag;

    // Part Map
    QMap<Part, class QLabel*> mPartIcon;
    QMap<QString, QList<Part>> mPathParts;
    QMap<Part, class QProgressBar*> mPartQual;
    QMap<Part, class QLabel*> mPartName;
    QMap<Part, class QLabel*> mPartLevel;
    QMap<Part, class QLabel*> mPartSet;
    QMap<Part, class QLabel*> mPartAttr;
    QMap<Part, class QLabel*> mPartEngrave;

    // Gem Map
    QList<class QLabel*> mGemIcons;
    QMap<QString, QList<int>> mGemPathIndex;
    QList<class QLabel*> mGemLevels;
    QList<class QLabel*> mGemNames;
    QList<class QLabel*> mGemAttrs;

    // Engrave
    QList<class QLabel*> mEngraveLabels;
    QList<class QHBoxLayout*> mEngraveLayouts;
    QList<class QHBoxLayout*> mPenaltyLayouts;

private slots:
    void slotProfileRequest();
    void slotExtractProfile(class QNetworkReply* reply);
    void slotSetIconEquip(class QNetworkReply* reply);
    void slotSetIconGem(class QNetworkReply* reply);
    void slotShowCharacterList();
};

#endif // PROFILE_H
