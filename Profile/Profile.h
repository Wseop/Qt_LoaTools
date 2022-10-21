#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QRegularExpression>

namespace Ui {
class Profile;
}

const QString PROFILE_PATH = "https://lostark.game.onstove.com/Profile/Character";
const QString CDN_PATH = "https://cdn-lostark.game.onstove.com";

enum class Part
{
    WEAPON = 0,
    HEAD,
    TOP,
    BOTTOM,
    HAND,
    SHOULDER,
    NECK,
    EAR1,
    EAR2,
    RING1,
    RING2,
    STONE,
    BRACELET = 26
};

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

private:
    void initMap();
    void initUI();
    void initConnect();

    QVariant getValueFromJson(const QJsonObject& src, QStringList keys);

    void parseEquip();
    void parseEngrave();
    void parseSkill();
    void parseCard();

    void updateEquip(QJsonObject& equip, QStringList& keys);
    void updateGem(QJsonObject& equip, QStringList& keys);

    void requestIcon(class QNetworkAccessManager* networkManager, QString iconPath);
    void setQualityColor(Part part, int quality);
    void setNameColor(class QLabel* label, int grade);
    int getItemGrade(const QJsonObject& obj);

    void clearAll();

private:
    Ui::Profile *ui;
    class QNetworkAccessManager* mNetworkProfile = nullptr;
    class QNetworkAccessManager* mNetworkIconEquip = nullptr;
    class QNetworkAccessManager* mNetworkIconGem = nullptr;
    class QJsonObject* mProfile = nullptr;

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

private slots:
    void slotProfileRequest();
    void slotExtractProfile(class QNetworkReply* reply);
    void slotSetIconEquip(class QNetworkReply* reply);
    void slotSetIconGem(class QNetworkReply* reply);
};

#endif // PROFILE_H
