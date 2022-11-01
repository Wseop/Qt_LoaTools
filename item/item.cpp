#include "item.h"

Item::Item(Part part) : mPart(part)
{

}

QString Item::getName() const
{
    return mName;
}

Grade Item::getGrade() const
{
    return mGrade;
}

QString Item::getIconPath() const
{
    return mIconPath;
}

Part Item::getPart() const
{
    return mPart;
}

void Item::setName(QString name)
{
    mName = name;
}

void Item::setGrade(Grade grade)
{
    mGrade = grade;
}

void Item::setIconPath(QString iconPath)
{
    mIconPath = iconPath;
}
