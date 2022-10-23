#ifndef BRACELET_H
#define BRACELET_H

#include "item.h"

class Bracelet : public Item
{
public:
    Bracelet();

    QString getEffect();

    void setEffect(QString effect);

private:
    QString mEffect;
};

#endif // BRACELET_H
