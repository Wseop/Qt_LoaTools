#ifndef BRACELET_H
#define BRACELET_H

#include "item.h"

class Bracelet : public Item
{
public:
    Bracelet();

    const QStringList& getEffects() const;

    void addEffect(const QString& effect);

private:
    QStringList m_effects;
};

#endif // BRACELET_H
