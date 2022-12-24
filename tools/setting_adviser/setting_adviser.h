#ifndef SETTING_ADVISER_H
#define SETTING_ADVISER_H

#include "game_data/profile/enum/class.h"

#include <QWidget>
#include <QJsonArray>

class SettingWidget;

namespace Ui {
class SettingAdviser;
}

class SettingAdviser : public QWidget
{
    Q_OBJECT

private:
    SettingAdviser();
    ~SettingAdviser();

    void setConnects();
    void setFonts();
    void setAlignments();
    void clearData();
    void setSettings(QJsonArray settings);
    void setSettingCounts();
    void renderSettings();
    SettingWidget* createSettingWidget(const QStringList& characters, QString settingCode, int rank, double adoptRate, int characterCount);

public:
    void setSelectedClass(Class cls);

public:
    static SettingAdviser* getInstance();
    static void destroyInstance();

private:
    Ui::SettingAdviser *ui;

    class DBRequest* m_pDBRequest;
    class ClassSelector* m_pClassSelector = nullptr;

    const int RENDER_COUNT = 30;
    Class m_selectedClass;
    QJsonArray m_settings;
    QList<QPair<QString, int>> m_settingCounts;
    QMap<QString, QStringList> m_settingCodeToCharacters;
    QList<SettingWidget*> m_settingWidgets;

    static SettingAdviser* m_pSettingAdviser;
};

#endif // SETTING_ADVISER_H
