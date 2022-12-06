#ifndef RUNE_H
#define RUNE_H

#include "item.h"

#include <QMap>

class Rune : public Item
{
public:
    Rune();

    void setName(QString name);

private:
    void initMap();
    void setIconPath(QString iconPath);

private:
    QMap<QString, int> m_runeId;
};

#endif // RUNE_H
