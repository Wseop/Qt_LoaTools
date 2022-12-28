#include "accessory_searcher.h"
#include "ui_accessory_searcher.h"
#include "font/font_manager.h"
#include "tools/accessory_searcher/auction_options.h"
#include "tools/accessory_searcher/ui/engrave_selector.h"
#include "tools/accessory_searcher/ui/penalty_selector.h"
#include "game_data/item/enum/item_grade.h"
#include "game_data/profile/enum/ability.h"

#include <QPushButton>

AccessorySearcher* AccessorySearcher::m_pInstance = nullptr;

AccessorySearcher::AccessorySearcher() :
    ui(new Ui::AccessorySearcher),
    m_pEngraveSelector1(new EngraveSelector()),
    m_pEngraveSelector2(new EngraveSelector()),
    m_pPenaltySelector(new PenaltySelector())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("악세 검색기");
    this->showMaximized();

    initQualityBtns();
    initItemGradeBtns();
    initAccessoryPartBtns();
    initAbilityBtns();
    initConnects();
    setFonts();
    setAlignments();
}

AccessorySearcher::~AccessorySearcher()
{
    for (auto* pBtn : m_qualityBtns)
        delete pBtn;
    for (auto* pBtn : m_itemGradeBtns)
        delete pBtn;
    for (auto* pBtn : m_accessoryPartBtns)
        delete pBtn;
    for (auto* pBtn : m_abilityBtns1)
        delete pBtn;
    for (auto* pBtn : m_abilityBtns2)
        delete pBtn;

    delete m_pEngraveSelector1;
    delete m_pEngraveSelector2;
    delete m_pPenaltySelector;

    delete ui;
}

void AccessorySearcher::initQualityBtns()
{
    QList<int> qualities = AuctionOptions::getInstance()->getItemGradeQualities();
    QString btnText = "%1 이상";

    for (const int& quality : qualities)
    {
        QPushButton* pButton = createButton(btnText.arg(quality));
        m_qualityBtns.append(pButton);
        ui->hLayoutQuality->addWidget(pButton);
        // TODO. connect
    }
}

void AccessorySearcher::initItemGradeBtns()
{
    QString colorStyle = "QPushButton { color: %1 }";

    for (int i = static_cast<int>(ItemGrade::일반); i <= static_cast<int>(ItemGrade::에스더); i++)
    {
        ItemGrade itemGrade = static_cast<ItemGrade>(i);
        QPushButton* pButton = createButton(itemGradeToStr(itemGrade));
        pButton->setStyleSheet(colorStyle.arg(colorCode(itemGrade)));
        m_itemGradeBtns.append(pButton);
        ui->hLayoutItemGrade->addWidget(pButton);
        // TODO. connect
    }
}

void AccessorySearcher::initAccessoryPartBtns()
{
    QList<int> accessoryCodes = AuctionOptions::getInstance()->getAccessoryCodes();
    QStringList accessoryParts = {"전체", "목걸이", "귀걸이", "반지"};

    for (int i = 0; i < accessoryCodes.size(); i++)
    {
        QPushButton* pButton = createButton(accessoryParts[i]);
        m_accessoryPartBtns.append(pButton);
        ui->hLayoutAccessoryPart->addWidget(pButton);
        // TODO. connect
    }
}

void AccessorySearcher::initAbilityBtns()
{
    // index is Ability
    QList<int> abilityCodes = AuctionOptions::getInstance()->getAbilityCodes();

    // Ability1
    for (int i = 0; i < abilityCodes.size(); i++)
    {
        QPushButton* pButton = createButton(abilityToStr(static_cast<Ability>(i)));
        m_abilityBtns1.append(pButton);
        ui->hLayoutAbilitySelect1->addWidget(pButton);
        // TODO. connect
    }
    // Ability2
    for (int i = 0; i < abilityCodes.size(); i++)
    {
        QPushButton* pButton = createButton(abilityToStr(static_cast<Ability>(i)));
        m_abilityBtns2.append(pButton);
        ui->hLayoutAbilitySelect2->addWidget(pButton);
        // TODO. connect
    }
}

void AccessorySearcher::initConnects()
{
    connect(ui->pbEngraving1Select, &QPushButton::pressed, this, [&](){
        m_pEngraveSelector1->show();
    });
    connect(ui->pbEngraving2Select, &QPushButton::pressed, this, [&](){
        m_pEngraveSelector2->show();
    });
    connect(ui->pbPenaltySelect, &QPushButton::pressed, this, [&](){
        m_pPenaltySelector->show();
    });
}

void AccessorySearcher::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->lbQuality->setFont(nanumBold10);
    ui->lbItemGrade->setFont(nanumBold10);
    ui->lbAccessoryPart->setFont(nanumBold10);
    ui->lbAbility1->setFont(nanumBold10);
    ui->leAbility1MinValue->setFont(nanumBold10);
    ui->leAbility1MaxValue->setFont(nanumBold10);
    ui->lbAbility1Tilde->setFont(nanumBold10);
    ui->lbAbility2->setFont(nanumBold10);
    ui->leAbility2MinValue->setFont(nanumBold10);
    ui->leAbility2MaxValue->setFont(nanumBold10);
    ui->lbAbility2Tilde->setFont(nanumBold10);
    ui->lbEngraving1->setFont(nanumBold10);
    ui->pbEngraving1Select->setFont(nanumBold10);
    ui->leEngraving1MinValue->setFont(nanumBold10);
    ui->leEngraving1MaxValue->setFont(nanumBold10);
    ui->lbEngraving1Tilde->setFont(nanumBold10);
    ui->lbEngraving2->setFont(nanumBold10);
    ui->pbEngraving2Select->setFont(nanumBold10);
    ui->leEngraving2MinValue->setFont(nanumBold10);
    ui->leEngraving2MaxValue->setFont(nanumBold10);
    ui->lbEngraving2Tilde->setFont(nanumBold10);
    ui->lbPenalty->setFont(nanumBold10);
    ui->pbPenaltySelect->setFont(nanumBold10);
    ui->lePenaltyMinValue->setFont(nanumBold10);
    ui->lePenaltyMaxValue->setFont(nanumBold10);
    ui->lbPenaltyTilde->setFont(nanumBold10);
    ui->pbSearch->setFont(nanumBold10);
    ui->pbClear->setFont(nanumBold10);
}

void AccessorySearcher::setAlignments()
{
    ui->vLayoutMain->setAlignment(Qt::AlignTop);
    ui->vLayoutSelector->setAlignment(Qt::AlignTop);
    ui->hLayoutQuality->setAlignment(Qt::AlignLeft);
    ui->hLayoutItemGrade->setAlignment(Qt::AlignLeft);
    ui->hLayoutAccessoryPart->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbility1->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbilitySelect1->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbilityValue1->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbility2->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbilitySelect2->setAlignment(Qt::AlignLeft);
    ui->hLayoutAbilityValue2->setAlignment(Qt::AlignLeft);
    ui->hLayoutEngraving1->setAlignment(Qt::AlignLeft);
    ui->hLayoutEngraving2->setAlignment(Qt::AlignLeft);
    ui->hLayoutPenalty->setAlignment(Qt::AlignLeft);
    ui->hLayoutSearchClear->setAlignment(Qt::AlignHCenter);
}

QPushButton* AccessorySearcher::createButton(QString text)
{
    QPushButton* pButton = new QPushButton(text);
    pButton->setFixedSize(100, 35);
    pButton->setFont(FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10));
    return pButton;
}

AccessorySearcher* AccessorySearcher::getInstance()
{
    if (m_pInstance == nullptr)
        m_pInstance = new AccessorySearcher();
    return m_pInstance;
}

void AccessorySearcher::destroyInstance()
{
    if (m_pInstance == nullptr)
        return;
    delete m_pInstance;
}
