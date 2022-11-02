#ifndef SKILL_H
#define SKILL_H

#include <QString>
#include "item/rune.h"

struct Tripod
{
    QString name;
    QString level;
    QString color;
};

class Skill
{
public:
    Skill();

    QString getName() const;
    QString getIconPath() const;
    int getLevel() const;
    QList<Tripod> getTripods() const;
    Tripod getTripod(int index) const;
    const Rune* getRune() const;

    void setName(QString name);
    void setIconPath(QString iconPath);
    void setLevel(int level);
    void setRune(Rune* rune);

    void addTripod(Tripod tripod);

private:
    QString mName;
    QString mIconPath;
    int mLevel;
    QList<Tripod> mTripods;
    Rune* mRune = nullptr;
};

#endif // SKILL_H
