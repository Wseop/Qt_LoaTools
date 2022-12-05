#ifndef ENGRAVE_H
#define ENGRAVE_H

#include <QStringList>

class Engrave
{
private:
    Engrave();
    ~Engrave();

    void loadEngraveList();

public:
    static Engrave* getInstance();
    static void destroyInstance();

    const QStringList& getEngraveList();
    const QStringList& getPenaltyList();
    QString getEngravePath(QString engrave) const;
    QString getPenaltyPath(QString penalty) const;

    bool isValidEngrave(QString engrave);
    bool isValidPenalty(QString penalty);
    bool isClassEngrave(QString engraveName);

    int indexOf(QString engraveName);
    QString getEngraveByIndex(int index);

private:
    static Engrave* m_pEngrave;

    QStringList m_engraves;
    QStringList m_classEngraves;
    QStringList m_penalties;
};

#endif // ENGRAVE_H
