#ifndef ITEM_H
#define ITEM_H

#include <QString>

enum class Grade
{
    NONE,
    UNCOMMON,
    RARE,
    EPIC,
    LEGEND,
    RELIC,
    ANCIENT,
    ESTHER,
};

enum class Part
{
    NONE = -1,
    WEAPON = 0,
    HEAD,
    TOP,
    BOTTOM,
    HAND,
    SHOULDER,
    NECK,
    EAR1,
    EAR2,
    RING1,
    RING2,
    STONE,
    BRACELET = 26,
    GEM,
};

class Item
{
public:
    Item(Part part);

    QString getName();
    Grade getGrade();
    QString getIconPath();
    Part getPart();

    void setName(QString name);
    void setGrade(Grade grade);
    void setIconPath(QString iconPath);

private:
    QString mName;
    Grade mGrade;
    QString mIconPath;
    Part mPart;
};

#endif // ITEM_H
