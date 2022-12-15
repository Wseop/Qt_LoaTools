#ifndef PROFILE_WIDGET_H
#define PROFILE_WIDGET_H

#include <QWidget>
#include <QLabel>

class Profile;

namespace Ui {
class ProfileWidget;
}

class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    ProfileWidget(QWidget* pParent, const Profile* pProfile);
    ~ProfileWidget();

private:
    void setLabels();
    void setAbilityLabel();
    void setFonts();

private:
    Ui::ProfileWidget *ui;
    QWidget* m_pParent;
    const Profile* m_pProfile;
    QList<QLabel*> m_abilityLabels;
};

#endif // PROFILE_WIDGET_H
