#include "spec_widget.h"
#include "ui_spec_widget.h"
#include "game_data/character/character.h"
#include "game_data/profile/profile.h"
#include "game_data/item/equip.h"
#include "game_data/item/accessory.h"
#include "game_data/item/gem.h"
#include "game_data/engrave/engrave.h"
#include "game_data/card/card.h"
#include "game_data/skill/skill.h"
#include "font/font_manager.h"
#include "tools/character_search/character_search.h"
#include "tools/character_search/ui/quality_color.h"
#include "tools/character_search/ui/engrave_widget.h"

const QString STYLE_UNDER_LEVEL = "QLabel { color: red }";
const int SPEC_CUT_ABILITY = 2100;
const int SPEC_CUT_GEM_AVG = 7;
const int SPEC_CUT_TRIPOD_ENABLED = 10;
const int SPEC_CUT_ENGRAVING = 15;

SpecWidget::SpecWidget() :
    ui(new Ui::SpecWidget),
    m_pCharacter(nullptr),
    m_pEngravingWidget(nullptr)
{
    ui->setupUi(this);

    setFonts();
    setAlignments();
    setConnects();
}

SpecWidget::~SpecWidget()
{
    delete m_pCharacter;
    delete m_pEngravingWidget;
    delete ui;
}

void SpecWidget::setCharacter(Character* pCharacter)
{
    m_pCharacter = pCharacter;

    setProfileData();
    setEngravingData();
    setGemData();
    setTripodData();
    setWeaponData();
    setAccData();
    setSetEffectData();
    setCardData();
}

void SpecWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumBold12 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 12);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbTitle->setFont(nanumBold10);
    ui->pbName->setFont(nanumBold10);
    ui->lbClass->setFont(nanumBold10);
    ui->lbExpedition->setFont(nanumBold10);
    ui->lbBattleLevel->setFont(nanumBold10);
    ui->lbItemLevel->setFont(nanumBold12);
    ui->lbCritical->setFont(nanumBold10);
    ui->lbSpecification->setFont(nanumBold10);
    ui->lbSwiftness->setFont(nanumBold10);
    ui->lbAbilitySum->setFont(nanumBold10);
    ui->pbEngraving->setFont(nanumBold10);
    ui->lbPenalty->setFont(nanumBold10);
    ui->lbGemMyul->setFont(nanumBold10);
    ui->lbGemHong->setFont(nanumBold10);
    ui->lbTripodLevel4->setFont(nanumBold10);
    ui->lbTripodLevel5->setFont(nanumBold10);
    ui->lbWeaponLevel->setFont(nanumBold12);
    ui->lbWeaponGrade->setFont(nanumBold10);
    ui->lbWeaponQual->setFont(nanumBold10);
    ui->lbAccQual->setFont(nanumBold10);
    ui->lbAccQualAvg->setFont(nanumBold10);
    ui->lbSetEffect->setFont(nanumBold10);
    ui->lbCard->setFont(nanumBold10);

    ui->groupTripod->setFont(nanumRegular10);
    ui->groupWeapon->setFont(nanumRegular10);
    ui->groupAcc->setFont(nanumRegular10);
    ui->groupSetEffect->setFont(nanumRegular10);
    ui->groupCard->setFont(nanumRegular10);
}

void SpecWidget::setAlignments()
{
    ui->hLayoutAcc->setAlignment(Qt::AlignHCenter);
}

void SpecWidget::setConnects()
{
    connect(ui->pbName, &QPushButton::pressed, this, [&](){
        CharacterSearch::getInstance()->changeCharacter(ui->pbName->text());
        CharacterSearch::getInstance()->show();
    });
    connect(ui->pbEngraving, &QPushButton::pressed, this, [&](){
        if (m_pEngravingWidget != nullptr)
            m_pEngravingWidget->show();
    });
}

void SpecWidget::setProfileData()
{
    const Profile* pProfile = m_pCharacter->getProfile();
    if (pProfile == nullptr)
        return;

    ui->lbTitle->setText(pProfile->getTitle());
    ui->pbName->setText(pProfile->getCharacterName());
    ui->lbClass->setText(classToStr(pProfile->getClass()));
    ui->lbExpedition->setText(QString("????????? Lv. %1").arg(pProfile->getExpLevel()));
    ui->lbBattleLevel->setText(QString("?????? Lv. %1").arg(pProfile->getCharacterLevel()));
    ui->lbItemLevel->setText(QString("????????? Lv. %1").arg(pProfile->getItemLevel()));

    const QMap<Ability, int>& abilityToValue = pProfile->getAbilities();
    int critical = abilityToValue[Ability::??????];
    int specification = abilityToValue[Ability::??????];
    int swiftness = abilityToValue[Ability::??????];
    int sumOfAbilities = critical + specification + swiftness;
    ui->lbCritical->setText(QString("?????? %1").arg(critical));
    ui->lbSpecification->setText(QString("?????? %1").arg(specification));
    ui->lbSwiftness->setText(QString("?????? %1").arg(swiftness));
    ui->lbAbilitySum->setText(QString("????????? %1").arg(sumOfAbilities));
    if (sumOfAbilities < SPEC_CUT_ABILITY)
        ui->lbAbilitySum->setStyleSheet(STYLE_UNDER_LEVEL);
}

void SpecWidget::setEngravingData()
{
    const Engrave* pEngrave = m_pCharacter->getEngrave();
    if (pEngrave == nullptr)
        return;

    const auto& engravings = pEngrave->getEngraves();
    QString engravingLevels = "?????? ";
    int sumOfLevels = 0;
    for (const auto& engraving : engravings)
    {
        int level = engraving.second;
        sumOfLevels += level;
        engravingLevels += QString::number(level);
    }
    if (sumOfLevels < SPEC_CUT_ENGRAVING)
        ui->pbEngraving->setStyleSheet("QPushButton { color: red }");
    ui->pbEngraving->setText(engravingLevels);

    const auto& penalties = pEngrave->getPenalties();
    QString penaltyLevels = "?????? ";
    if (penalties.size() == 0)
    {
        penaltyLevels += "-";
    }
    else
    {
        for (const auto& penalty : penalties)
        {
            penaltyLevels += QString::number(penalty.second);
        }
        ui->lbPenalty->setStyleSheet(STYLE_UNDER_LEVEL);
    }
    ui->lbPenalty->setText(penaltyLevels);

    m_pEngravingWidget = new EngraveWidget(this, pEngrave);
}

void SpecWidget::setGemData()
{
    const auto& gems = m_pCharacter->getGems();
    int myulCount = 0;
    int sumOfMyulLevel = 0;
    double avgOfMyulLevel = 0;
    int hongCount = 0;
    int sumOfHongLevel = 0;
    double avgOfHongLevel = 0;

    for (const auto* pGem : gems)
    {
        GemType gemType = pGem->getGemType();
        if (gemType == GemType::??????)
        {
            myulCount++;
            sumOfMyulLevel += pGem->getLevel();
        }
        else if (gemType == GemType::??????)
        {
            hongCount++;
            sumOfHongLevel += pGem->getLevel();
        }
    }

    QString underLevelStyle = "QLabel { color: red }";
    QString myulText = QString("%1 ??????").arg(myulCount);
    if (myulCount != 0)
    {
        avgOfMyulLevel = sumOfMyulLevel / (double)myulCount;
        myulText += QString(" (?????? Lv. %1)").arg(avgOfMyulLevel, 0, 'f', 2, QChar(' '));
    }
    ui->lbGemMyul->setText(myulText);
    if (!isSupporter(m_pCharacter->getProfile()->getClass()) && avgOfMyulLevel < SPEC_CUT_GEM_AVG)
        ui->lbGemMyul->setStyleSheet(STYLE_UNDER_LEVEL);

    QString hongText = QString("%1 ??????").arg(hongCount);
    if (hongCount != 0)
    {
        avgOfHongLevel = sumOfHongLevel / (double)hongCount;
        hongText += QString(" (?????? Lv. %1)").arg(avgOfHongLevel, 0, 'f', 2, QChar(' '));
    }
    ui->lbGemHong->setText(hongText);
    if (avgOfHongLevel < SPEC_CUT_GEM_AVG)
        ui->lbGemHong->setStyleSheet(STYLE_UNDER_LEVEL);
}

void SpecWidget::setTripodData()
{
    const auto& skills = m_pCharacter->getSkills();
    int tripodLevel4Count = 0;
    int tripodLevel5Count = 0;

    for (const auto* pSkill : skills)
    {
        const auto& tripods = pSkill->getTripods();
        for (const auto& tripod : tripods)
        {
            int tripodLevel = tripod.level;
            if (tripodLevel == 4)
                tripodLevel4Count++;
            else if (tripodLevel == 5)
                tripodLevel5Count++;
        }
    }

    ui->lbTripodLevel4->setText(QString("Lv.4 %1???").arg(tripodLevel4Count));
    ui->lbTripodLevel5->setText(QString("Lv.5 %1???").arg(tripodLevel5Count));
    ui->groupTripod->setTitle(QString("??????????????? (%1/18)").arg(tripodLevel4Count + tripodLevel5Count));

    if ((tripodLevel4Count + tripodLevel5Count) < SPEC_CUT_TRIPOD_ENABLED && !isSupporter(m_pCharacter->getProfile()->getClass()))
    {
        ui->lbTripodLevel4->setStyleSheet(STYLE_UNDER_LEVEL);
        ui->lbTripodLevel5->setStyleSheet(STYLE_UNDER_LEVEL);
    }
}

void SpecWidget::setWeaponData()
{
    const Equip* pWeapon = m_pCharacter->getEquip(ItemType::??????);
    if (pWeapon == nullptr)
        return;

    // ?????? ?????? ??? ????????? ?????? parsing
    QString name = pWeapon->getName();
    QString levelText;
    int reforgeStartIndex = name.indexOf("+");
    int reforgeEndIndex = name.indexOf(" ");
    if (reforgeStartIndex == -1 || reforgeEndIndex == -1)
        return;
    levelText += name.sliced(reforgeStartIndex, reforgeEndIndex - reforgeStartIndex);

    QString levelTier = pWeapon->getLevelTier();
    int levelStartIndex = levelTier.indexOf("??????") + 3;
    int levelEndIndex = levelTier.indexOf("(");
    if (levelStartIndex == -1 || levelEndIndex == -1)
        return;
    levelText += QString(" (%1)").arg(levelTier.sliced(levelStartIndex, levelEndIndex - levelStartIndex - 1));
    ui->lbWeaponLevel->setText(levelText);

    // ?????? (????????? color, ????????? ??????)
    ItemGrade weaponGrade = pWeapon->getGrade();
    QString gradeText = itemGradeToStr(weaponGrade);
    QString gradeColor = QString("QLabel { color: %1 }").arg(colorCode(weaponGrade));
    if (pWeapon->isElla())
    {
        gradeText += " (??????)";
    }
    ui->lbWeaponGrade->setText(gradeText);
    ui->lbWeaponGrade->setStyleSheet(gradeColor);

    // ??????
    int quality = pWeapon->getQuality();
    QString qualityText = QString::number(quality);
    QString qualityColor = QString("QLabel { color: %1 }").arg(getQualityColor(quality));
    ui->lbWeaponQual->setText(qualityText);
    ui->lbWeaponQual->setStyleSheet(qualityColor);
}

void SpecWidget::setAccData()
{
    const Accessory* pAccessory = nullptr;
    QString accQualText;
    int sumOfQualities = 0;

    pAccessory = m_pCharacter->getAccessory(ItemType::?????????);
    if (pAccessory == nullptr)
    {
        accQualText += "-";
    }
    else
    {
        accQualText += QString::number(pAccessory->getQuality());
        sumOfQualities += pAccessory->getQuality();
    }
    accQualText += " ";

    for (int i = static_cast<int>(ItemType::?????????); i <= static_cast<int>(ItemType::??????); i++)
    {
        for (int j = 0; j < 2; j++)
        {
            pAccessory = m_pCharacter->getAccessory(static_cast<ItemType>(i), j);
            if (pAccessory == nullptr)
            {
                accQualText += "-";
            }
            else
            {
                accQualText += QString::number(pAccessory->getQuality());
                sumOfQualities += pAccessory->getQuality();
            }
            accQualText += " ";
        }
    }

    double accQualAvg = sumOfQualities / (double)5;
    QString accQualAvgText = QString("(?????? %1)").arg(accQualAvg, 0, 'f', 2, QChar(' '));
    ui->lbAccQual->setText(accQualText);
    ui->lbAccQualAvg->setText(accQualAvgText);

    QString colorStyle = QString("QLabel { color: %1 }").arg(getQualityColor(static_cast<int>(accQualAvg)));
    ui->lbAccQualAvg->setStyleSheet(colorStyle);
}

void SpecWidget::setSetEffectData()
{
    const Equip* pEquip = nullptr;
    QList<int> setEffectCounts(static_cast<int>(SetEffect::Size), 0);
    QString setLevel;
    bool bFullSetEnabled = true;

    for (int i = static_cast<int>(ItemType::??????); i <= static_cast<int>(ItemType::??????); i++)
    {
        pEquip = m_pCharacter->getEquip(static_cast<ItemType>(i));
        if (pEquip == nullptr)
            continue;

        if (pEquip->getGrade() == ItemGrade::?????????)
        {
            pEquip = m_pCharacter->getEquip(ItemType::??????);
            if (pEquip == nullptr)
                continue;
        }

        SetEffect setEffect = pEquip->getSetEffect();
        if (setEffect == SetEffect::Size)
        {
            bFullSetEnabled = false;
            continue;
        }
        setEffectCounts[static_cast<int>(setEffect)]++;
        setLevel += QString::number(pEquip->getSetLevel());
    }

    QString setEffectText;
    for (int i = 0; i < setEffectCounts.size(); i++)
    {
        if (setEffectCounts[i] != 0)
        {
            setEffectText += QString("%1%2 ").arg(setEffectCounts[i]).arg(setEffectToStr(static_cast<SetEffect>(i)));
        }
    }
    setEffectText += QString(" (%1)").arg(setLevel);

    ui->lbSetEffect->setText(setEffectText);
    if (!bFullSetEnabled)
        ui->lbSetEffect->setStyleSheet(STYLE_UNDER_LEVEL);
}

void SpecWidget::setCardData()
{
    const auto& cards = m_pCharacter->getCards();
    QString cardText;

    for (int i = 0; i < cards.size(); i++)
    {
        const Card* pCard = cards[i];
        QString setName = pCard->getEffectNames()[pCard->getEffectNames().size() - 1];
        int nameStartIndex = 0;
        int nameEndIndex = setName.indexOf("??????") - 2;
        int awakenStartIndex = setName.indexOf("(");
        int awakenEndIndex = setName.indexOf("????????????");
        if (nameEndIndex == -1)
            continue;

        cardText += setName.sliced(nameStartIndex, nameEndIndex - nameStartIndex);
        if (awakenStartIndex == -1 || awakenEndIndex == -1)
        {
            cardText += " 0???";
        }
        else
        {
            awakenStartIndex += 1;
            cardText += QString(" %1???").arg(setName.sliced(awakenStartIndex, awakenEndIndex - awakenStartIndex));
        }

        if (i != cards.size() - 1)
            cardText += "\n";
    }

    ui->lbCard->setText(cardText);
}
