#include "profile_widget.h"
#include "ui_profile_widget.h"
#include "game_data/profile/profile.h"

ProfileWidget::ProfileWidget(QWidget* parent, const Profile* profile) :
    ui(new Ui::ProfileWidget),
    m_pParent(parent),
    m_pProfile(profile)
{
    ui->setupUi(this);

    setTexts();
}

ProfileWidget::~ProfileWidget()
{
    for (QLabel* label : m_abilityLabels)
        delete label;
    m_abilityLabels.clear();

    delete ui;
}

void ProfileWidget::setTexts()
{
    ui->lbServer->setText("@ " + m_pProfile->getServer());
    ui->lbClass->setText(classToStr(m_pProfile->getClass()));
    ui->lbCharacterLevel->setText(QString("전투 Lv. %1").arg(m_pProfile->getCharacterLevel()));
    ui->lbCharacterName->setText(m_pProfile->getCharacterName());
    ui->lbExpLevel->setText(QString("원정대 Lv. %1").arg(m_pProfile->getExpLevel()));
    ui->lbItemLevel->setText(QString("아이템 Lv. %1").arg(m_pProfile->getItemLevel()));
    ui->lbTitle->setText(m_pProfile->getTitle());
    ui->lbGuild->setText(m_pProfile->getGuild());
    setAbilities();
}

void ProfileWidget::setAbilities()
{
    const QMap<Ability, int>& abilityToValue = m_pProfile->getAbilities();

    for (int i = 0; i < static_cast<int>(Ability::Size); i++)
    {
        int col = i / 3;
        if (col == 1)
            col = 2;

        Ability ability = static_cast<Ability>(i);
        int value = abilityToValue[ability];
        QLabel* labelAbility = new QLabel(abilityToStr(ability));
        labelAbility->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_abilityLabels.append(labelAbility);
        ui->gridAbility->addWidget(labelAbility, i % 3, col);

        QLabel* labelValue = new QLabel(QString(" %1").arg(value));
        labelValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_abilityLabels.append(labelValue);
        ui->gridAbility->addWidget(labelValue, i % 3, col + 1);
    }
}
