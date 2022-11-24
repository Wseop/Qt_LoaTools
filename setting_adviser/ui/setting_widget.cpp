#include "setting_widget.h"
#include "ui_setting_widget.h"

SettingWidget::SettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);
}

SettingWidget::~SettingWidget()
{
    QList<QLabel*> classIcons = m_mapClassIcon.keys();
    for (QLabel* classIcon : classIcons)
    {
        m_mapClassIcon[classIcon]->removeWidget(classIcon);
        delete classIcon;
    }

    QList<QLabel*> classEngraves = m_mapClassEngrave.keys();
    for (QLabel* classEngrave : classEngraves)
    {
        m_mapClassEngrave[classEngrave]->removeWidget(classEngrave);
        delete classEngrave;
    }

    QList<QHBoxLayout*> classLayouts = m_mapClassLayout.keys();
    for (QHBoxLayout* classLayout : classLayouts)
    {
        m_mapClassLayout[classLayout]->removeItem(classLayout);
        delete classLayout;
    }

    QList<QLabel*> normalIcons = m_mapNormalIcon.keys();
    for (QLabel* normalIcon : normalIcons)
    {
        m_mapNormalIcon[normalIcon]->removeWidget(normalIcon);
        delete normalIcon;
    }

    QList<QLabel*> normalEngraves = m_mapNormalEngrave.keys();
    for (QLabel* normalEngrave : normalEngraves)
    {
        m_mapNormalEngrave[normalEngrave]->removeWidget(normalEngrave);
        delete normalEngrave;
    }

    QList<QVBoxLayout*> normalLayouts = m_mapNormalLayout.keys();
    for (QVBoxLayout* normalLayout : normalLayouts)
    {
        m_mapNormalLayout[normalLayout]->removeItem(normalLayout);
        delete normalLayout;
    }

    delete ui;
}

void SettingWidget::setIndex(int index)
{
    m_index = index;
}

void SettingWidget::setNumOfCharacters(int numOfCharacters)
{
    m_numOfCharacters = numOfCharacters;
}

void SettingWidget::setAdoptRatio(double adoptRatio)
{
    m_adoptRatio = adoptRatio;

    QString titleText = QString("%1) 채용률 : %2% (%3 캐릭터)").arg(m_index).arg(m_adoptRatio * 100, 0, 'f', 2, QChar(' ')).arg(m_numOfCharacters);
    ui->groupSetting->setTitle(titleText);
}

void SettingWidget::setAbilities(QStringList abilities)
{
    m_abilities = abilities;

    renderAbility();
}

void SettingWidget::setSetEffects(QStringList setEffects)
{
    m_setEffects = setEffects;

    renderSetEffect();
}

void SettingWidget::setClassEngraves(QList<QPair<QString, int> > classEngraves)
{
    m_classEngraves = classEngraves;

    renderClassEngrave();
}

void SettingWidget::setNormalEngraves(QList<QPair<QString, int> > normalEngraves)
{
    m_normalEngraves = normalEngraves;

    renderNormalEngrave();
}

void SettingWidget::renderAbility()
{
    ui->lbNeck1->setText(m_abilities[0]);
    ui->lbNeck2->setText(m_abilities[1]);
    ui->lbEar1->setText(m_abilities[2]);
    ui->lbEar2->setText(m_abilities[3]);
    ui->lbRing1->setText(m_abilities[4]);
    ui->lbRing2->setText(m_abilities[5]);
}

void SettingWidget::renderSetEffect()
{
    QString text;
    for (int i = 0; i < m_setEffects.size(); i++)
    {
        if (i == 0)
            text += m_setEffects[i];
        else
            text += "\n" + m_setEffects[i];
    }
    ui->lbSetEffect->setText(text);
}

void SettingWidget::renderClassEngrave()
{
    for (const auto& classEngrave : m_classEngraves)
    {
        QHBoxLayout* layout = new QHBoxLayout();
        ui->vLayoutClassEngrave->addLayout(layout);
        m_mapClassLayout[layout] = ui->vLayoutClassEngrave;

        QLabel* lbIcon = new QLabel();
        layout->addWidget(lbIcon);
        m_mapClassIcon[lbIcon] = layout;
        lbIcon->setFixedSize(50, 50);
        QPixmap icon;
        if (icon.load(m_engrave.getEngravePath(classEngrave.first)))
            lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        QLabel* lbEngrave = new QLabel();
        layout->addWidget(lbEngrave);
        m_mapClassEngrave[lbEngrave] = layout;
        lbEngrave->setContentsMargins(10, 0, 0, 0);
        lbEngrave->setFont(QFont("나눔스퀘어 네오 Bold", 12));
        lbEngrave->setText(QString("%1 Lv.%2").arg(classEngrave.first).arg(classEngrave.second));
    }
}

void SettingWidget::renderNormalEngrave()
{
    for (const auto& normalEngrave : m_normalEngraves)
    {
        QVBoxLayout* layout = new QVBoxLayout();
        ui->hLayoutNormalEngrave->addLayout(layout);
        m_mapNormalLayout[layout] = ui->hLayoutNormalEngrave;
        layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        QLabel* lbIcon = new QLabel();
        layout->addWidget(lbIcon);
        m_mapNormalIcon[lbIcon] = layout;
        lbIcon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QPixmap icon;
        if (icon.load(m_engrave.getEngravePath(normalEngrave.first)))
            lbIcon->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        QLabel* lbEngrave = new QLabel();
        layout->addWidget(lbEngrave);
        m_mapNormalEngrave[lbEngrave] = layout;
        lbEngrave->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        lbEngrave->setFont(QFont("나눔스퀘어 네오 Bold", 10));
        lbEngrave->setText(QString("%1 Lv.%2").arg(normalEngrave.first).arg(normalEngrave.second));
    }

    if (is97Stone())
    {
        QString title = ui->groupSetting->title();
        title += " - (97돌 필요)";
        ui->groupSetting->setTitle(title);
    }
}

bool SettingWidget::is97Stone()
{
    // 9,7 or 10,6 어빌리티 스톤 세팅인 경우 true 반환
    QList<int> engraveLevels;
    for (const auto& normalEngrave : m_normalEngraves)
        engraveLevels.append(normalEngrave.second);
    for (const auto& classEngrave : m_classEngraves)
        engraveLevels.append(classEngrave.second);

    std::sort(engraveLevels.begin(), engraveLevels.end(), [](int a, int b) {
        return a > b;
    });

    QString engraveLevelStr;
    for (const int& engraveLevel : engraveLevels)
        engraveLevelStr += QString("%1").arg(engraveLevel);

    return engraveLevelStr == "333332";
}
