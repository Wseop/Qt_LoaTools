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
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbTitle->setFont(nanumBold10);
    ui->pbName->setFont(nanumBold10);
    ui->lbClass->setFont(nanumBold10);
    ui->lbExpedition->setFont(nanumBold10);
    ui->lbBattleLevel->setFont(nanumBold10);
    ui->lbItemLevel->setFont(nanumBold10);
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
    ui->lbWeaponLevel->setFont(nanumBold10);
    ui->lbWeaponGrade->setFont(nanumBold10);
    ui->lbWeaponQual->setFont(nanumBold10);
    ui->lbAccQual->setFont(nanumBold10);
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
    ui->lbExpedition->setText(QString("원정대 Lv. %1").arg(pProfile->getExpLevel()));
    ui->lbBattleLevel->setText(QString("전투 Lv. %1").arg(pProfile->getCharacterLevel()));
    ui->lbItemLevel->setText(QString("아이템 Lv. %1").arg(pProfile->getItemLevel()));

    const QMap<Ability, int>& abilityToValue = pProfile->getAbilities();
    int critical = abilityToValue[Ability::치명];
    int specification = abilityToValue[Ability::특화];
    int swiftness = abilityToValue[Ability::신속];
    ui->lbCritical->setText(QString("치명 %1").arg(critical));
    ui->lbSpecification->setText(QString("특화 %1").arg(specification));
    ui->lbSwiftness->setText(QString("신속 %1").arg(swiftness));
    ui->lbAbilitySum->setText(QString("특성합 %1").arg(critical + specification + swiftness));
}

void SpecWidget::setEngravingData()
{
    const Engrave* pEngrave = m_pCharacter->getEngrave();
    if (pEngrave == nullptr)
        return;

    const auto& engravings = pEngrave->getEngraves();
    QString engravingLevels = "각인 ";
    for (const auto& engraving : engravings)
    {
        engravingLevels += QString::number(engraving.second);
    }
    ui->pbEngraving->setText(engravingLevels);

    const auto& penalties = pEngrave->getPenalties();
    QString penaltyLevels = "감소 ";
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
    }
    ui->lbPenalty->setText(penaltyLevels);

    m_pEngravingWidget = new EngraveWidget(this, pEngrave);
}

void SpecWidget::setGemData()
{
    const auto& gems = m_pCharacter->getGems();
    int myulCount = 0;
    int sumOfMyulLevel = 0;
    int hongCount = 0;
    int sumOfHongLevel = 0;

    for (const auto* pGem : gems)
    {
        GemType gemType = pGem->getGemType();
        if (gemType == GemType::멸화)
        {
            myulCount++;
            sumOfMyulLevel += pGem->getLevel();
        }
        else if (gemType == GemType::홍염)
        {
            hongCount++;
            sumOfHongLevel += pGem->getLevel();
        }
    }

    QString myulText = QString("%1 멸화").arg(myulCount);
    if (myulCount != 0)
    {
        myulText += QString(" (평균 Lv. %1)").arg(sumOfMyulLevel / (double)myulCount, 0, 'f', 2, QChar(' '));
    }
    ui->lbGemMyul->setText(myulText);

    QString hongText = QString("%1 홍염").arg(hongCount);
    if (hongCount != 0)
    {
        hongText += QString(" (평균 Lv. %1)").arg(sumOfHongLevel / (double)hongCount, 0, 'f', 2, QChar(' '));
    }
    ui->lbGemHong->setText(hongText);
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

    ui->lbTripodLevel4->setText(QString("Lv.4 %1개").arg(tripodLevel4Count));
    ui->lbTripodLevel5->setText(QString("Lv.5 %1개").arg(tripodLevel5Count));
}

void SpecWidget::setWeaponData()
{
    const Equip* pWeapon = m_pCharacter->getEquip(ItemType::무기);
    if (pWeapon == nullptr)
        return;

    // 강화 수치 및 아이템 레벨 parsing
    QString name = pWeapon->getName();
    QString levelText;
    int reforgeStartIndex = name.indexOf("+");
    int reforgeEndIndex = name.indexOf(" ");
    if (reforgeStartIndex == -1 || reforgeEndIndex == -1)
        return;
    levelText += name.sliced(reforgeStartIndex, reforgeEndIndex - reforgeStartIndex);

    QString levelTier = pWeapon->getLevelTier();
    int levelStartIndex = levelTier.indexOf("레벨") + 3;
    int levelEndIndex = levelTier.indexOf("(");
    if (levelStartIndex == -1 || levelEndIndex == -1)
        return;
    levelText += QString(" (%1)").arg(levelTier.sliced(levelStartIndex, levelEndIndex - levelStartIndex - 1));
    ui->lbWeaponLevel->setText(levelText);

    // 등급 (문자열 color, 에스더 엘라)
    ItemGrade weaponGrade = pWeapon->getGrade();
    QString gradeText = itemGradeToStr(weaponGrade);
    QString gradeColor = QString("QLabel { color: %1 }").arg(colorCode(weaponGrade));
    if (pWeapon->isElla())
    {
        gradeText += " (엘라)";
    }
    ui->lbWeaponGrade->setText(gradeText);
    ui->lbWeaponGrade->setStyleSheet(gradeColor);

    // 품질
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

    pAccessory = m_pCharacter->getAccessory(ItemType::목걸이);
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

    for (int i = static_cast<int>(ItemType::귀걸이); i <= static_cast<int>(ItemType::반지); i++)
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

    accQualText += QString("(평균 %1)").arg(sumOfQualities / (double)5, 0, 'f', 2, QChar(' '));
    ui->lbAccQual->setText(accQualText);
}

void SpecWidget::setSetEffectData()
{
    const Equip* pEquip = nullptr;
    QList<int> setEffectCounts(static_cast<int>(SetEffect::Size), 0);
    QString setLevel;

    for (int i = static_cast<int>(ItemType::무기); i <= static_cast<int>(ItemType::어깨); i++)
    {
        pEquip = m_pCharacter->getEquip(static_cast<ItemType>(i));
        if (pEquip == nullptr)
            continue;

        if (pEquip->getGrade() == ItemGrade::에스더)
        {
            pEquip = m_pCharacter->getEquip(ItemType::장갑);
            if (pEquip == nullptr)
                continue;
        }

        SetEffect setEffect = pEquip->getSetEffect();
        if (setEffect == SetEffect::Size)
            continue;
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
        int nameEndIndex = setName.indexOf("세트") - 2;
        int awakenStartIndex = setName.indexOf("(") + 1;
        int awakenEndIndex = setName.indexOf("각성합계");
        if (nameEndIndex == -1)
            continue;

        cardText += setName.sliced(nameStartIndex, nameEndIndex - nameStartIndex);
        if (awakenStartIndex == -1)
        {
            cardText += " 0각";
        }
        else
        {
            cardText += QString(" %1각").arg(setName.sliced(awakenStartIndex, awakenEndIndex - awakenStartIndex));
        }

        if (i != cards.size() - 1)
            cardText += "\n";
    }

    ui->lbCard->setText(cardText);
}
