#include "card_set.h"

CardSet::CardSet()
{

}

QString CardSet::getTitle(int index) const
{
    return mTitleList[index];
}

QString CardSet::getDesc(int index) const
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

qsizetype CardSet::count() const
{
    return mTitleList.count();
}
