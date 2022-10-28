#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "item.h"

class Accessory : public Item
{
public:
    Accessory(Part part);

    int getQuality();
    QString getAttr();
    QString getEngrave();

    void setQuality(int quality);
    void setAttr(QString attr);
    void setEngrave(QString engrave);

private:
    int mQuality;
    QString mAttr;
    QString mEngrave;
};

#endif // ACCESSORY_H