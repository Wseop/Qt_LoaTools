#ifndef SKILLRUNEMANAGER_H
#define SKILLRUNEMANAGER_H

#include <QMap>
#include <QString>

class SkillRuneManager
{
private:
    SkillRuneManager();
    ~SkillRuneManager();

    void initRuneData();

public:
    static SkillRuneManager* getInstance();
    static void destroyInstance();

    QString getRuneIconPath(QString runeName);

private:
    static SkillRuneManager* m_pSkillRuneManager;

    QMap<QString, int> m_runeIndex;
};

#endif // SKILLRUNEMANAGER_H
