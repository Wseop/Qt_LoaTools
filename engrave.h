#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QStringList>
#include <QMap>

const QString PATH_ENGRAVE = ":/image/resources/engraves";

class Engrave
{
public:
    Engrave();

    const QStringList& getEngraveList();
    const QStringList& getPenaltyList();
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

    bool isClassEngrave(QString engraveName);
    int indexOf(QString engraveName);
    QString getEngraveByIndex(int index);

private:
    void loadEngraveList();

private:
    QStringList m_engraves;
    QStringList m_penalties;
    QMap<QString, int> m_engraveValue;
    QMap<QString, int> m_penaltyValue;

    QStringList m_classEngraves;
};

#endif // ENGRAVE_H
