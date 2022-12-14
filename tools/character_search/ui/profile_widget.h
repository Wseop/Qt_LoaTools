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
    ProfileWidget(QWidget* parent, const Profile* profile);
    ~ProfileWidget();

private:
    void initFonts();
    void initStyleSheets();
    void setTexts();
    void setAbilities();

private:
    Ui::ProfileWidget *ui;
    QWidget* m_pParent;
    const Profile* m_pProfile;
    QList<QLabel*> m_abilityLabels;
};

#endif // PROFILE_WIDGET_H
