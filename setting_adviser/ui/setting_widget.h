#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include "engrave.h"

#include <QWidget>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();

    void setIndex(int index);
    void setNumOfCharacters(int numOfCharacters);
    void setAdoptRatio(double adoptRatio);
    void setAbilities(QStringList abilities);
    void setSetEffects(QStringList setEffects);
    void setClassEngraves(QList<QPair<QString, int>> classEngraves);
    void setNormalEngraves(QList<QPair<QString, int>> normalEngraves);

private:
    void renderAbility();
    void renderSetEffect();
    void renderClassEngrave();
    void renderNormalEngrave();

    bool is97Stone();

private:
    Ui::SettingWidget *ui;

    int m_index;
    int m_numOfCharacters;
    double m_adoptRatio;
    QStringList m_abilities;
    QStringList m_setEffects;
    QList<QPair<QString, int>> m_classEngraves;
    QList<QPair<QString, int>> m_normalEngraves;

    Engrave m_engrave;

    QMap<class QHBoxLayout*, class QVBoxLayout*> m_mapClassLayout;
    QMap<class QLabel*, class QHBoxLayout*> m_mapClassIcon;
    QMap<class QLabel*, class QHBoxLayout*> m_mapClassEngrave;
    QMap<class QVBoxLayout*, class QHBoxLayout*> m_mapNormalLayout;
    QMap<class QLabel*, class QVBoxLayout*> m_mapNormalIcon;
    QMap<class QLabel*, class QVBoxLayout*> m_mapNormalEngrave;
};

#endif // SETTING_WIDGET_H
