#ifndef GEM_H
#define GEM_H

#include "item.h"

class Gem : public Item
{
public:
    Gem();

    int getLevel() const;
    QString getAttr() const;
    QString getEffect() const;

    void setLevel(int level);
    void setEffect(const QString& effect);

private:
    int m_level;
    QString m_effect;
};

#endif // GEM_H
