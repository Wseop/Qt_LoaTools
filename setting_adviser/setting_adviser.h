#ifndef SETTING_ADVISER_H
#define SETTING_ADVISER_H

#include "enum/class.h"
#include "engrave.h"

#include <QWidget>

namespace Ui {
class SettingAdviser;
}

class QNetworkReply;

class SettingAdviser : public QWidget
{
    Q_OBJECT

public:
    explicit SettingAdviser(QWidget *parent = nullptr);
    ~SettingAdviser();

    void setSelectedClass(QString cls);

private:
    void initConnect();

    void readSettingsByClass(QString cls);
    void renderSettings();

private slots:
    void slotShowClassSelector();
    void slotHandleReplySetSettings(QNetworkReply* reply);

private:
    Ui::SettingAdviser *ui;

    const int MAX_RENDER_COUNT = 10;

    QWidget* m_pParent = nullptr;
    class ClassSelector* m_pClassSelector = nullptr;

    Class m_selectedClass;
    int m_numOfCharacters;
    QList<QPair<QString, int>> m_settingCodes;
    Engrave m_engrave;

    QMap<class SettingWidget*, class QVBoxLayout*> m_mapSettingWidgetLayout;
};

#endif // SETTING_ADVISER_H
