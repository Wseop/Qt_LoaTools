#ifndef GEM_H
#define GEM_H

#include "item.h"

class Gem : public Item
{
public:
    Gem();

    QString GetLevel();
    QString getAttr();

    void setLevel(QString level);
    void setAttr(QString attr);

private:
    QString mLevel;
    QString mAttr;
};

#endif // GEM_H
