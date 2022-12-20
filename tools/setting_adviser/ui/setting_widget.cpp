#include "setting_widget.h"
#include "ui_setting_widget.h"
#include "font/font_manager.h"
#include "db/document/settingcode_manager.h"
#include "game_data/engrave/engrave_manager.h"

#include <QPixmap>

SettingWidget::SettingWidget(QString settingCode, int rank, double adoptRate, int characterCount) :
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    setAdoptRate(rank, adoptRate, characterCount);
    setAbility(SettingCodeManager::parseAbilities(settingCode));
    setSetEffect(SettingCodeManager::parseSetEffects(settingCode));
    setEngrave(SettingCodeManager::parseClassEngraves(settingCode), SettingCodeManager::parseNormalEngraves(settingCode));
    setFonts();
    setAlignments();
}

SettingWidget::~SettingWidget()
{
    for (QLabel* pLabel : m_iconLabels)
        delete pLabel;

    delete ui;
}

void SettingWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont nanumRegular10 = pFontManager->getFont(FontFamily::NanumSquareNeoRegular, 10);

    ui->lbEngraveLevel->setFont(nanumBold10);
    ui->lbEngraveName->setFont(nanumBold10);
    ui->lbRate->setFont(nanumBold10);
    ui->lbNeck1->setFont(nanumBold10);
    ui->lbNeck2->setFont(nanumBold10);
    ui->lbEar1->setFont(nanumBold10);
    ui->lbEar2->setFont(nanumBold10);
    ui->lbRing1->setFont(nanumBold10);
    ui->lbRing2->setFont(nanumBold10);
    ui->lbSetEffect->setFont(nanumBold10);

    ui->groupEngrave->setFont(nanumRegular10);
    ui->groupRate->setFont(nanumRegular10);
    ui->groupAbility->setFont(nanumRegular10);
    ui->groupEar->setFont(nanumRegular10);
    ui->groupNeck->setFont(nanumRegular10);
    ui->groupRing->setFont(nanumRegular10);
    ui->groupSetEffect->setFont(nanumRegular10);
}

void SettingWidget::setAlignments()
{

}

void SettingWidget::setAdoptRate(int rank, double adoptRate, int characterCount)
{
    QString text = QString("%1순위) %2% (%3개 캐릭터)").arg(rank).arg(adoptRate, 0, 'f', 2, QChar(' ')).arg(characterCount);
    ui->lbRate->setText(text);
}

void SettingWidget::setAbility(QList<Ability> abilities)
{
    ui->lbNeck1->setText(abilityToStr(abilities[0]));
    ui->lbNeck2->setText(abilityToStr(abilities[1]));
    ui->lbEar1->setText(abilityToStr(abilities[2]));
    ui->lbEar2->setText(abilityToStr(abilities[3]));
    ui->lbRing1->setText(abilityToStr(abilities[4]));
    ui->lbRing2->setText(abilityToStr(abilities[5]));
}

void SettingWidget::setSetEffect(QList<SetEffect> setEffects)
{
    QMap<SetEffect, int> setEffectToCount;
    for (const SetEffect& setEffect : setEffects)
    {
        setEffectToCount[setEffect]++;
    }

    QList<QPair<SetEffect, int>> setEffectCounts(setEffectToCount.keyValueBegin(), setEffectToCount.keyValueEnd());
    std::sort(setEffectCounts.begin(), setEffectCounts.end(), [&](QPair<SetEffect, int> a, QPair<SetEffect, int> b){
        return a.second < b.second;
    });

    QString text;
    for (const auto& setEffectCount : setEffectCounts)
    {
        text += QString("%1%2 ").arg(setEffectCount.second).arg(setEffectToStr(setEffectCount.first));
    }
    ui->lbSetEffect->setText(text);
}

void SettingWidget::setEngrave(QList<QPair<QString, int>> classEngraves, QList<QPair<QString, int>> normalEngraves)
{
    QString nameText;
    QString levelText;

    for (int level = 3; level > 0; level--)
    {
        for (const auto& normalEngrave : normalEngraves)
        {
            QString engraveName = normalEngrave.first;
            int engraveLevel = normalEngrave.second;
            if (engraveLevel != level)
                continue;

            QLabel* pIconLabel = createIconLabel(engraveName);
            ui->hLayoutEngraveIcon->addWidget(pIconLabel);
            nameText += engraveName[0];
            levelText += QString::number(engraveLevel);
        }

        for (const auto& classEngrave : classEngraves)
        {
            QString engraveName = classEngrave.first;
            int engraveLevel = classEngrave.second;
            if (engraveLevel != level)
                continue;

            QLabel* pIconLabel = createIconLabel(engraveName);
            ui->hLayoutEngraveIcon->addWidget(pIconLabel);
            nameText += engraveName[0];
            levelText += QString::number(engraveLevel);
        }
    }

    ui->lbEngraveName->setText(nameText);
    ui->lbEngraveLevel->setText(levelText);
}

QLabel* SettingWidget::createIconLabel(QString engrave)
{
    QLabel* pIconLabel = new QLabel();
    pIconLabel->setFixedSize(50, 50);
    m_iconLabels.append(pIconLabel);

    QPixmap icon;
    if (icon.load(EngraveManager::getInstance()->getEngravePath(engrave)))
    {
        pIconLabel->setPixmap(icon.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        pIconLabel->setStyleSheet("QLabel { border: 1px solid black }");
    }

    return pIconLabel;
}
