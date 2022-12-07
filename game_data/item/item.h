#ifndef ITEM_H
#define ITEM_H

#include "game_data/item/enum/item_grade.h"
#include "game_data/item/enum/item_type.h"

#include <QString>
#include <QStringList>

class Item
{
public:
    Item(ItemType type);

    QString getName() const;
    ItemGrade getGrade() const;
    QString getIconPath() const;
    ItemType getType() const;

    void setName(QString name);
    void setGrade(ItemGrade grade);
    void setIconPath(QString iconPath);

private:
    QString m_name;
    ItemGrade m_grade;
    QString m_iconPath;
    ItemType m_type;
};

#endif // ITEM_H
