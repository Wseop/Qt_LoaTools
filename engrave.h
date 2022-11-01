#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QStringList>
#include <QMap>

const QString PATH_ENGRAVE = ":/image/resources/engraves";

class Engrave
{
public:
    Engrave();

    QStringList getEngraveList();
    QStringList getPenaltyList();
    bool isValidEngrave(QString engrave);
    bool isValidPenalty(QString penalty);
    QString getEngravePath(QString engrave) const;
    QString getPenaltyPath(QString penalty) const;

    void addEngrave(QString engrave, int value);
    void addPenalty(QString penalty, int value);
    QStringList getActiveEngraveList() const;
    QStringList getActivePenaltyList() const;
    int getEngraveValue(QString engrave) const;
    int getPenaltyValue(QString penalty) const;
    void clearEngrave();
    void clearPenalty();

private:
    void loadEngraveList();

private:
    QStringList mEngraveList;
    QStringList mPenaltyList;
    QMap<QString, int> mEngraveValue;
    QMap<QString, int> mPenaltyValue;
};

#endif // ENGRAVE_H
