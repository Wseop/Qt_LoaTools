#include "accessory_searcher.h"
#include "ui_accessory_searcher.h"
#include "font/font_manager.h"
#include "tools/accessory_searcher/auction_options.h"
#include "tools/accessory_searcher/search_filter.h"
#include "tools/accessory_searcher/ui/engrave_selector.h"
#include "tools/accessory_searcher/ui/penalty_selector.h"
#include "game_data/item/enum/item_grade.h"
#include "game_data/profile/enum/ability.h"

#include <QPushButton>
#include <QIntValidator>

AccessorySearcher* AccessorySearcher::m_pInstance = nullptr;

AccessorySearcher::AccessorySearcher() :
    ui(new Ui::AccessorySearcher),
    m_pPenaltySelector(new PenaltySelector()),
    m_pSearchFilter(new SearchFilter()),
    m_engravingToCode(AuctionOptions::getInstance()->getEngravingCodes()),
    m_penaltyToCode(AuctionOptions::getInstance()->getPenaltyCodes())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("악세 검색기");
    this->showMaximized();

    for (int i = 0; i < 2; i++)
    {
        m_engraveSelectors.append(new EngraveSelector(i));
    }

    initQualityBtns();
    initItemGradeBtns();
    initAccessoryPartBtns();
    initAbilityBtns();
    initLineEditValidators();
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
    for (auto* pEngraveSelector : m_engraveSelectors)
        delete pEngraveSelector;
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
        connect(pButton, &QPushButton::pressed, this, [&, quality](){
            m_pSearchFilter->setQuality(quality);
            // TODO. ui
        });
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
        connect(pButton, &QPushButton::pressed, this, [&, itemGrade](){
            m_pSearchFilter->setItemGrade(itemGradeToStr(itemGrade));
            // TODO. ui
        });
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

        int accessoryCode = accessoryCodes[i];
        connect(pButton, &QPushButton::pressed, this, [&, accessoryCode](){
            m_pSearchFilter->setCategory(accessoryCode);
            // TODO. ui
        });
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

        int abilityCode = abilityCodes[i];
        connect(pButton, &QPushButton::pressed, this, [&, abilityCode](){
            m_pSearchFilter->setAbilityCode1(abilityCode);
            // TODO. ui
        });
    }
    // Ability2
    for (int i = 0; i < abilityCodes.size(); i++)
    {
        QPushButton* pButton = createButton(abilityToStr(static_cast<Ability>(i)));
        m_abilityBtns2.append(pButton);
        ui->hLayoutAbilitySelect2->addWidget(pButton);

        int abilityCode = abilityCodes[i];
        connect(pButton, &QPushButton::pressed, this, [&, abilityCode](){
            m_pSearchFilter->setAbilityCode2(abilityCode);
            // TODO. ui
        });
    }
}

void AccessorySearcher::initLineEditValidators()
{
    QIntValidator* pValidator = new QIntValidator(0, 99);

    ui->leAbilityMinValue1->setValidator(pValidator);
    ui->leAbilityMaxValue1->setValidator(pValidator);
    ui->leAbilityMinValue2->setValidator(pValidator);
    ui->leAbilityMaxValue2->setValidator(pValidator);
    ui->leEngravingMinValue1->setValidator(pValidator);
    ui->leEngravingMaxValue1->setValidator(pValidator);
    ui->leEngravingMinValue2->setValidator(pValidator);
    ui->leEngravingMaxValue2->setValidator(pValidator);
    ui->lePenaltyMinValue->setValidator(pValidator);
    ui->lePenaltyMaxValue->setValidator(pValidator);
}

void AccessorySearcher::initConnects()
{
    connect(ui->pbEngraving1Select, &QPushButton::pressed, this, [&](){
        m_engraveSelectors[0]->show();
    });
    connect(ui->pbEngraving2Select, &QPushButton::pressed, this, [&](){
        m_engraveSelectors[1]->show();
    });
    connect(ui->pbPenaltySelect, &QPushButton::pressed, this, [&](){
        m_pPenaltySelector->show();
    });
    connect(ui->pbSearch, &QPushButton::pressed, this, [&](){
        updateSearchFilter();
        qDebug() << m_pSearchFilter->getFilterObj();
    });
    connect(ui->pbClear, &QPushButton::pressed, this, &AccessorySearcher::initFilter);
}

void AccessorySearcher::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    ui->lbQuality->setFont(nanumBold10);
    ui->lbItemGrade->setFont(nanumBold10);
    ui->lbAccessoryPart->setFont(nanumBold10);
    ui->lbAbility1->setFont(nanumBold10);
    ui->leAbilityMinValue1->setFont(nanumBold10);
    ui->leAbilityMaxValue1->setFont(nanumBold10);
    ui->lbAbility1Tilde->setFont(nanumBold10);
    ui->lbAbility2->setFont(nanumBold10);
    ui->leAbilityMinValue2->setFont(nanumBold10);
    ui->leAbilityMaxValue2->setFont(nanumBold10);
    ui->lbAbility2Tilde->setFont(nanumBold10);
    ui->lbEngraving1->setFont(nanumBold10);
    ui->pbEngraving1Select->setFont(nanumBold10);
    ui->leEngravingMinValue1->setFont(nanumBold10);
    ui->leEngravingMaxValue1->setFont(nanumBold10);
    ui->lbEngraving1Tilde->setFont(nanumBold10);
    ui->lbEngraving2->setFont(nanumBold10);
    ui->pbEngraving2Select->setFont(nanumBold10);
    ui->leEngravingMinValue2->setFont(nanumBold10);
    ui->leEngravingMaxValue2->setFont(nanumBold10);
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

void AccessorySearcher::updateSearchFilter()
{
    QString value;

    // ability1 min, max
    value = ui->leAbilityMinValue1->text();
    if (value == "")
        m_pSearchFilter->setAbilityMinValue1();
    else
        m_pSearchFilter->setAbilityMinValue1(value.toInt());

    value = ui->leAbilityMaxValue1->text();
    if (value == "")
        m_pSearchFilter->setAbilityMaxValue1();
    else
        m_pSearchFilter->setAbilityMaxValue1(value.toInt());

    // ability2 min, max
    value = ui->leAbilityMinValue2->text();
    if (value == "")
        m_pSearchFilter->setAbilityMinValue2();
    else
        m_pSearchFilter->setAbilityMinValue2(value.toInt());

    value = ui->leAbilityMaxValue2->text();
    if (value == "")
        m_pSearchFilter->setAbilityMaxValue2();
    else
        m_pSearchFilter->setAbilityMaxValue2(value.toInt());

    // engraving1 min, max
    value = ui->leEngravingMinValue1->text();
    if (value == "")
        m_pSearchFilter->setEngravingMinValue1();
    else
        m_pSearchFilter->setEngravingMinValue1(value.toInt());

    value = ui->leEngravingMaxValue1->text();
    if (value == "")
        m_pSearchFilter->setEngravingMaxValue1();
    else
        m_pSearchFilter->setEngravingMaxValue1(value.toInt());

    // engraving2 min, max
    value = ui->leEngravingMinValue2->text();
    if (value == "")
        m_pSearchFilter->setEngravingMinValue2();
    else
        m_pSearchFilter->setEngravingMinValue2(value.toInt());

    value = ui->leEngravingMaxValue2->text();
    if (value == "")
        m_pSearchFilter->setEngravingMaxValue2();
    else
        m_pSearchFilter->setEngravingMaxValue2(value.toInt());

    // penalty min, max
    value = ui->lePenaltyMinValue->text();
    if (value == "")
        m_pSearchFilter->setPenaltyMinValue();
    else
        m_pSearchFilter->setPenaltyMinValue(value.toInt());

    value = ui->lePenaltyMaxValue->text();
    if (value == "")
        m_pSearchFilter->setPenaltyMaxValue();
    else
        m_pSearchFilter->setPenaltyMaxValue(value.toInt());

    // engraving
    QString engraving;

    engraving = ui->pbEngraving1Select->text();
    if (engraving != "각인 선택")
        m_pSearchFilter->setEngraving1(m_engravingToCode[engraving]);

    engraving = ui->pbEngraving2Select->text();
    if (engraving != "각인 선택")
        m_pSearchFilter->setEngraving2(m_engravingToCode[engraving]);

    engraving = ui->pbPenaltySelect->text();
    if (engraving != "각인 선택")
        m_pSearchFilter->setPenalty(m_penaltyToCode[engraving]);
}

void AccessorySearcher::initFilter()
{
    delete m_pSearchFilter;
    m_pSearchFilter = new SearchFilter();

    ui->pbEngraving1Select->setText("각인 선택");
    ui->pbEngraving2Select->setText("각인 선택");
    ui->pbPenaltySelect->setText("각인 선택");
    ui->leAbilityMinValue1->clear();
    ui->leAbilityMaxValue1->clear();
    ui->leAbilityMinValue2->clear();
    ui->leAbilityMaxValue2->clear();
    ui->leEngravingMinValue1->clear();
    ui->leEngravingMaxValue1->clear();
    ui->leEngravingMinValue2->clear();
    ui->leEngravingMaxValue2->clear();
    ui->lePenaltyMinValue->clear();
    ui->lePenaltyMaxValue->clear();
}

QPushButton* AccessorySearcher::createButton(QString text)
{
    QPushButton* pButton = new QPushButton(text);
    pButton->setFixedSize(100, 35);
    pButton->setFont(FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10));
    return pButton;
}

void AccessorySearcher::setEngraving(int buttonIndex, QString engraving)
{
    if (buttonIndex == 0)
        ui->pbEngraving1Select->setText(engraving);
    else if (buttonIndex == 1)
        ui->pbEngraving2Select->setText(engraving);
}

void AccessorySearcher::setPenalty(QString penalty)
{
    ui->pbPenaltySelect->setText(penalty);
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
