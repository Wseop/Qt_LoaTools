#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QStringList>

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

static QStringList gItemColor = {
    "#000000",
    "#8DF901",
    "#00B0FA",
    "#CE43FC",
    "#F99200",
    "#FA5D00",
    "#E3C7A1",
    "#3CF2E6"
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
    RUNE,
};

class Item
{
public:
    Item(Part part);

    QString getName() const;
    Grade getGrade() const;
    QString getIconPath() const;
    Part getPart() const;

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
