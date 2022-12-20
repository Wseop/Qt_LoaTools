#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include "game_data/profile/enum/ability.h"
#include "game_data/item/enum/set_effect.h"

#include <QWidget>
#include <QLabel>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    SettingWidget(QString settingCode, int rank, double adoptRate, int characterCount);
    ~SettingWidget();

private:
    void setFonts();
    void setAlignments();
    void setAdoptRate(int rank, double adoptRate, int characterCount);
    void setAbility(QList<Ability> abilities);
    void setSetEffect(QList<SetEffect> setEffects);
    void setEngrave(QList<QPair<QString, int>> classEngraves, QList<QPair<QString, int>> normalEngraves);
    QLabel* createIconLabel(QString engrave);

private:
    Ui::SettingWidget *ui;

    QList<QLabel*> m_iconLabels;
};

#endif // SETTING_WIDGET_H
