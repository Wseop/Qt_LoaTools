#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QStringList>

const QString PATH_ENGRAVE = ":/image/resources/engraves";

class Engrave
{
public:
    Engrave();

    QStringList getEngraveList();
    QStringList getPenaltyList();
    bool isValidEngrave(QString engrave);
    bool isValidPenalty(QString penalty);
    QString getEngravePath(QString engrave);
    QString getPenaltyPath(QString penalty);

private:
    void loadEngraveList();

private:
    QStringList mEngraveList;
    QStringList mPenaltyList;
};

#endif // ENGRAVE_H
