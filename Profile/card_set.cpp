#include "card_set.h"

CardSet::CardSet()
{

}

QString CardSet::getTitle(int index)
{
    return mTitleList[index];
}

QString CardSet::getDesc(int index)
{
    return mDescList[index];
}

void CardSet::addTitle(QString title)
{
    mTitleList << title;
}

void CardSet::addDesc(QString desc)
{
    mDescList << desc;
}

qsizetype CardSet::count()
{
    return mTitleList.count();
}
