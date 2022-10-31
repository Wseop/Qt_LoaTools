#ifndef CARDSET_H
#define CARDSET_H

#include <QStringList>

class CardSet
{
public:
    CardSet();

    QString getTitle(int index);
    QString getDesc(int index);

    void addTitle(QString title);
    void addDesc(QString desc);

    qsizetype count();

private:
    QStringList mTitleList;
    QStringList mDescList;
};

#endif // CARDSET_H
