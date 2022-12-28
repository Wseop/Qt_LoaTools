#include "auction_options.h"
#include "http_client/http_client.h"
#include "game_data/profile/enum/ability.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

AuctionOptions* AuctionOptions::m_pInstance = nullptr;

AuctionOptions::AuctionOptions() :
    m_pNetworkManager(new QNetworkAccessManager())
{
    loadAuctionOptions();
}

AuctionOptions::~AuctionOptions()
{
    delete m_pNetworkManager;
}

QList<int> AuctionOptions::getItemGradeQualities()
{
    const QJsonArray& qualities = m_auctionOptions.find("ItemGradeQualities")->toArray();
    QList<int> itemGradeQualities;

    for (const QJsonValue& quality : qualities)
    {
        itemGradeQualities.append(quality.toInt());
    }

    return itemGradeQualities;
}

QList<int> AuctionOptions::getAbilityCodes()
{
    const QJsonArray& etcOptions = m_auctionOptions.find("EtcOptions")->toArray();
    const QJsonArray& etcSubs = etcOptions[0].toObject().find("EtcSubs")->toArray();
    QList<int> abilityCodes(static_cast<int>(Ability::Size));

    for (const QJsonValue& etcSub : etcSubs)
    {
        const QJsonObject& etcSubObj = etcSub.toObject();
        int abilityCode = etcSubObj.find("Value")->toInt();
        Ability ability = strToAbility(etcSubObj.find("Text")->toString());

        abilityCodes[static_cast<int>(ability)] = abilityCode;
    }

    return abilityCodes;
}

QMap<QString, int> AuctionOptions::getEngravingCodes()
{
    const QJsonArray& etcOptions = m_auctionOptions.find("EtcOptions")->toArray();
    const QJsonArray& etcSubs = etcOptions[1].toObject().find("EtcSubs")->toArray();
    QMap<QString, int> engravingCodes;

    for (const QJsonValue& etcSub : etcSubs)
    {
        const QJsonObject& etcSubObj = etcSub.toObject();
        int engravingCode = etcSubObj.find("Value")->toInt();
        QString engraving = etcSubObj.find("Text")->toString();

        engravingCodes[engraving] = engravingCode;
    }

    return engravingCodes;
}

QList<int> AuctionOptions::getAccessoryCodes()
{
    const QJsonObject& categoriesObj = m_auctionOptions.find("Categories")->toArray()[2].toObject();
    QList<int> accessoryCodes;

    // '장신구' 코드 추가
    accessoryCodes.append(categoriesObj.find("Code")->toInt());

    const QJsonArray& subs = categoriesObj.find("Subs")->toArray();
    for (const QJsonValue& sub : subs)
    {
        // 목걸이, 귀걸이, 반지 코드 추가
        const QJsonObject& subObj = sub.toObject();
        QString name = subObj.find("CodeName")->toString();
        if (name == "목걸이" || name == "귀걸이" || name == "반지")
        {
            accessoryCodes.append(subObj.find("Code")->toInt());
        }
    }

    return accessoryCodes;
}

AuctionOptions* AuctionOptions::getInstance()
{
    if (m_pInstance == nullptr)
        m_pInstance = new AuctionOptions();
    return m_pInstance;
}

void AuctionOptions::destroyInstance()
{
    if (m_pInstance == nullptr)
        return;
    delete m_pInstance;
}

void AuctionOptions::loadAuctionOptions()
{
    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply* pReply){
        m_auctionOptions = QJsonDocument::fromJson(pReply->readAll()).object();
    });

    QStringList dummyParam;
    HttpClient::getInstance()->sendGetRequest(m_pNetworkManager, LostarkApi::AuctionOptions, 2, dummyParam);
}
