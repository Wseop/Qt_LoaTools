#ifndef SETTING_ADVISER_H
#define SETTING_ADVISER_H

#include "enum/class.h"
#include "engrave.h"

#include <QWidget>

namespace Ui {
class SettingAdviser;
}

class QNetworkReply;

using SettingCodeCount = QPair<QString, int>;

class SettingAdviser : public QWidget
{
    Q_OBJECT

public:
    explicit SettingAdviser(QWidget *parent = nullptr);
    ~SettingAdviser();

    void setSelectedClass(QString cls);

private:
    void initConnect();
    void clearData();

    void requestSettingsBySelectedClass();
    void renderSettings();
    void renderTopSettings();

private slots:
    void slotHandleSettingsBySelectedClass(QVariantList jsonSettings);
    void slotRequestAllSettings();
    void slotShowClassSelector();
    void slotHandleAllSettings(QVariantList jsonSettings);

private:
    Ui::SettingAdviser *ui;

    const int MAX_RENDER_COUNT = 10;

    QWidget* m_pParent = nullptr;
    class ClassSelector* m_pClassSelector = nullptr;

    Class m_selectedClass;
    int m_numOfCharacters;
    QList<SettingCodeCount> m_settingCodes;
    QList<QList<SettingCodeCount>> m_topSettingCodes;
    QList<int> m_classCounts;
    Engrave m_engrave;

    QMap<class SettingWidget*, class QVBoxLayout*> m_mapSettingWidgetLayout;
};

#endif // SETTING_ADVISER_H
