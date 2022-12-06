#ifndef ACCESSORY_H
#define ACCESSORY_H

#include "item.h"
#include <QStringList>

class Accessory : public Item
{
public:
    Accessory(ItemPart part);

    int getQuality() const;
    QString getAbility() const;
    const QStringList& getEngraves() const;
    QString getPenalty() const;

    void setQuality(int quality);
    void setAbility(QString ability);
    void addEngrave(QString engrave);
    void setPenalty(QString penalty);

private:
    int m_quality;
    QString m_ability;
    QStringList m_engraves;
    QString m_penalty;
};

#endif // ACCESSORY_H
