#ifndef CARDSET_H
#define CARDSET_H

#include <QStringList>

class CardSet
{
public:
    CardSet();

    QString getTitle(int index) const;
    QString getDesc(int index) const;

    void addTitle(QString title);
    void addDesc(QString desc);

    qsizetype count() const;

private:
    QStringList mTitleList;
    QStringList mDescList;
};

#endif // CARDSET_H
