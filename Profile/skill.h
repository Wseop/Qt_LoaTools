#ifndef SKILL_H
#define SKILL_H

#include <QString>
#include "item/rune.h"

struct Tripod
{
    QString name;
    QString iconPath;
    QString level;
};

class Skill
{
public:
    Skill();

    QString getName();
    QString getIconPath();
    int getLevel();
    QList<Tripod> getTripods();
    Tripod getTripod(int index);
    Rune getRune();

    void setName(QString name);
    void setIconPath(QString iconPath);
    void setLevel(int level);
    void setTripod(QList<Tripod> tripods);
    void setRune(Rune rune);

    void addTripod(Tripod tripod);

private:
    QString mName;
    QString mIconPath;
    int mLevel;
    QList<Tripod> mTripods;
    Rune mRune;
};

#endif // SKILL_H
