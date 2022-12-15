#include "profile_widget.h"
#include "ui_profile_widget.h"
#include "game_data/profile/profile.h"
#include "font/font_manager.h"

ProfileWidget::ProfileWidget(QWidget* pParent, const Profile* pProfile) :
    ui(new Ui::ProfileWidget),
    m_pParent(pParent),
    m_pProfile(pProfile)
{
    ui->setupUi(this);

    setLabels();
    setFonts();
}

ProfileWidget::~ProfileWidget()
{
    for (QLabel* pLabel : m_abilityLabels)
        delete pLabel;
    m_abilityLabels.clear();

    delete ui;
}

void ProfileWidget::setLabels()
{
    QString labelColor = QString("QLabel { color: %1 }");

    ui->lbServer->setText("@ " + m_pProfile->getServer());
    ui->lbServer->setStyleSheet(labelColor.arg("#B178FF"));

    ui->lbClass->setText(classToStr(m_pProfile->getClass()));
    ui->lbCharacterLevel->setText(QString("전투 Lv. %1").arg(m_pProfile->getCharacterLevel()));

    ui->lbCharacterName->setText(m_pProfile->getCharacterName());

    ui->lbExpLevel->setText(QString("원정대 Lv. %1").arg(m_pProfile->getExpLevel()));
    ui->lbExpLevel->setStyleSheet(labelColor.arg("#F7B838"));
    ui->lbItemLevel->setText(QString("아이템 Lv. %1").arg(m_pProfile->getItemLevel()));
    ui->lbItemLevel->setStyleSheet(labelColor.arg("#FF009B"));

    ui->lbTitle->setText(m_pProfile->getTitle());

    ui->lbGuild->setText(m_pProfile->getGuild());

    setAbilityLabel();
}

void ProfileWidget::setAbilityLabel()
{
    const QMap<Ability, int>& abilityToValue = m_pProfile->getAbilities();

    for (int i = 0; i < static_cast<int>(Ability::Size); i++)
    {
        int col = i / 3;
        if (col == 1)
            col = 2;

        Ability ability = static_cast<Ability>(i);
        int value = abilityToValue[ability];
        QLabel* pLabelAbility = new QLabel(abilityToStr(ability));
        pLabelAbility->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_abilityLabels.append(pLabelAbility);
        ui->gridAbility->addWidget(pLabelAbility, i % 3, col);

        QLabel* pLabelValue = new QLabel(QString(" %1").arg(value));
        pLabelValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_abilityLabels.append(pLabelValue);
        ui->gridAbility->addWidget(pLabelValue, i % 3, col + 1);
    }
}

void ProfileWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbServer->setFont(nanumBold10);
    ui->lbClass->setFont(nanumBold10);
    ui->lbCharacterLevel->setFont(nanumBold10);
    ui->lbCharacterName->setFont(nanumBold10);
    ui->lbExpLevel->setFont(nanumBold10);
    ui->lbItemLevel->setFont(nanumBold10);
    ui->lbTitle->setFont(nanumBold10);
    ui->lbGuild->setFont(nanumBold10);
    for (QLabel* pLabel : m_abilityLabels)
        pLabel->setFont(nanumBold10);

    ui->groupTitle->setFont(nanumRegular10);
    ui->groupGuild->setFont(nanumRegular10);
    ui->groupAbility->setFont(nanumRegular10);
}
