#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QString>
#include <QList>

class Engrave
{
public:
    Engrave();

    const QList<QPair<QString, int>>& getEngraves() const;
    const QList<QPair<QString, int>>& getPenalties() const;

    void addEngrave(const QString& engrave, int level);
    void addPenalty(const QString& penalty, int level);

private:
    QList<QPair<QString, int>> m_engraves;
    QList<QPair<QString, int>> m_penalties;
};

#endif // ENGRAVE_H
