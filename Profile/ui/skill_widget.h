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
    explicit SkillWidget(QWidget *parent = nullptr, const Skill* skill = nullptr, QString iconUrl = "");
    ~SkillWidget();

private:
    void requestSkillIcon(QString iconUrl);
    void setSkillName();
    void setTripods();
    void setRuneIcon();
    void setRuneName();

private:
    Ui::SkillWidget *ui;
    const Skill* mSkill;
    QNetworkAccessManager* mNetworkManager;
    QList<QLabel*> mTripods;

private slots:
    void slotUpdateSkillIcon(QNetworkReply* reply);
};

#endif // SKILL_WIDGET_H
