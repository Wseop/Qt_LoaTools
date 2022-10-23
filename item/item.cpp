#include "item.h"

Item::Item(Part part) : mPart(part)
{

}

QString Item::getName()
{
    return mName;
}

Grade Item::getGrade()
{
    return mGrade;
}

QString Item::getIconPath()
{
    return mIconPath;
}

Part Item::getPart()
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
