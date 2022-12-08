#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QString>
#include <QMap>

class Engrave
{
public:
    Engrave();

    const QMap<QString, int>& getEngraves() const;
    const QMap<QString, int>& getPenalties() const;

    void addEngrave(const QString& engrave, int level);
    void addPenalty(const QString& penalty, int level);

private:
    // Key : engrave_name, Value : engrave_level
    QMap<QString, int> m_engraves;
    QMap<QString, int> m_penalties;
};

#endif // ENGRAVE_H
