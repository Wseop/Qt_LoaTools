#include "item.h"

Item::Item(ItemPart part) :
    m_grade(ItemGrade::NONE),
    m_part(part)
{

}

QString Item::getName() const
{
    return m_name;
}

ItemGrade Item::getGrade() const
{
    return m_grade;
}

QString Item::getIconPath() const
{
    return m_iconPath;
}

ItemPart Item::getPart() const
{
    return m_part;
}

void Item::setName(QString name)
{
    m_name = name;
}

void Item::setGrade(ItemGrade grade)
{
    m_grade = grade;
}

void Item::setIconPath(QString iconPath)
{
    m_iconPath = iconPath;
}
