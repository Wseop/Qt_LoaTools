#ifndef SKILL_WIDGET_H
#define SKILL_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class Skill;

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QWidget
{
    Q_OBJECT

public:
    SkillWidget(QWidget* pParent, const Skill* pSkill);
    ~SkillWidget();

private:
    void setTexts();
    void setTripods();
    void setRune();
    void loadSkillIcon();
    void loadRuneIcon(QString iconPath, QLabel* pLbRuneIcon);
    void setFonts();

private:
    Ui::SkillWidget *ui;
    QWidget* m_pParent;
    const Skill* m_pSkill;
    QList<class QNetworkAccessManager*> m_networkManagers;
    QList<QLabel*> m_labels;
    QList<QHBoxLayout*> m_hLayouts;
};

#endif // SKILL_WIDGET_H
