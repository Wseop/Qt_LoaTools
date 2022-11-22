#ifndef SETTING_ADVISER_H
#define SETTING_ADVISER_H

#include "enum/class.h"

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

private slots:
    void slotShowClassSelector();
    void slotHandleReplySetSettings(QNetworkReply* reply);

    void slotTest();

private:
    Ui::SettingAdviser *ui;

    QWidget* m_pParent = nullptr;
    class ClassSelector* m_pClassSelector = nullptr;

    Class m_selectedClass;
    QList<QPair<QString, int>> m_settingCodes;
};

#endif // SETTING_ADVISER_H
