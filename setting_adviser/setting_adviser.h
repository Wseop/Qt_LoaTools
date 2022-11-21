#ifndef SETTING_ADVISER_H
#define SETTING_ADVISER_H

#include <QWidget>

namespace Ui {
class SettingAdviser;
}

class SettingAdviser : public QWidget
{
    Q_OBJECT

public:
    explicit SettingAdviser(QWidget *parent = nullptr);
    ~SettingAdviser();

    void setSelectedClass(QString cls);

private:
    void initConnect();

private slots:
    void slotShowClassSelector();

private:
    Ui::SettingAdviser *ui;

    QWidget* m_pParent = nullptr;
    class ClassSelector* m_pClassSelector = nullptr;
};

#endif // SETTING_ADVISER_H
