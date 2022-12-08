#ifndef ENGRAVE_MANAGER_H
#define ENGRAVE_MANAGER_H

#include <QStringList>

class EngraveManager
{
private:
    EngraveManager();
    ~EngraveManager();

    void loadEngraveList();

public:
    static EngraveManager* getInstance();
    static void destroyInstance();
    static QStringList extractActiveEngraves(const QMap<QString, int>& engraveValues);
    static QStringList extractActivePenalties(const QMap<QString, int>& penaltyValues);

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
    static EngraveManager* m_pEngrave;

    QStringList m_engraves;
    QStringList m_classEngraves;
    QStringList m_penalties;
};

#endif // ENGRAVE_MANAGER_H
