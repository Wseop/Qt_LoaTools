#ifndef SKILL_WIDGET_H
#define SKILL_WIDGET_H

#include <QWidget>

class Skill;
class QNetworkAccessManager;
class QNetworkReply;
class QLabel;

namespace Ui {
class SkillWidget;
}

class SkillWidget : public QWidget
{
    Q_OBJECT

public:
    SkillWidget(const Skill* skill = nullptr, QString iconUrl = "");
    ~SkillWidget();

private:
    void initFont();
    void initAlignment();

    void requestSkillIcon(QString iconUrl);
    void setSkillName();
    void setTripods();
    void setRuneIcon();
    void setRuneName();

private:
    Ui::SkillWidget *ui;
    const Skill* m_pSkill;
    QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_tripods;

private slots:
    void slotUpdateSkillIcon(QNetworkReply* reply);
};

#endif // SKILL_WIDGET_H
