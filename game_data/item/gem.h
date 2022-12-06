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
    int m_level;
    QString m_attr;
    QString m_effect;
};

#endif // GEM_H
