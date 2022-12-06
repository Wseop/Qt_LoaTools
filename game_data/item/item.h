#ifndef ITEM_H
#define ITEM_H

#include "game_data/item/enum/item_grade.h"
#include "game_data/item/enum/item_part.h"

#include <QString>
#include <QStringList>

static QStringList gItemColor = {
    "#000000",
    "#00B700",
    "#00B0FA",
    "#CE43FC",
    "#F99200",
    "#FA5D00",
    "#DCB496",
    "#3CF2E6"
};

class Item
{
public:
    Item(ItemPart part);

    QString getName() const;
    ItemGrade getGrade() const;
    QString getIconPath() const;
    ItemPart getPart() const;

    void setName(QString name);
    void setGrade(ItemGrade grade);
    void setIconPath(QString iconPath);

private:
    QString m_name;
    ItemGrade m_grade;
    QString m_iconPath;
    ItemPart m_part;
};

#endif // ITEM_H
