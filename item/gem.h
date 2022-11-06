#ifndef GEM_H
#define GEM_H

#include "item.h"

class Gem : public Item
{
public:
    Gem();

    int GetLevel() const;
    QString getAttr() const;
    QString getEffect() const;

    void setLevel(int level);
    void setAttr(QString attr);
    void setEffect(QString effect);

private:
    int mLevel;
    QString mAttr;
    QString mEffect;
};

#endif // GEM_H
