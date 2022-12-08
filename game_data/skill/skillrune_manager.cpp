#include "skillrune_manager.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

SkillRuneManager* SkillRuneManager::m_pSkillRuneManager = nullptr;

SkillRuneManager::SkillRuneManager()
{
    initRuneData();
}

SkillRuneManager::~SkillRuneManager()
{
    destroyInstance();
}

void SkillRuneManager::initRuneData()
{
    QString filePath = QDir::currentPath() + "/resources/rune_list.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << ": File open fail";
        return;
    }

    QTextStream inStream(&file);
    int index = 0;
    while (!inStream.atEnd())
        m_runeIndex[inStream.readLine()] = index++;

    file.close();
}

SkillRuneManager* SkillRuneManager::getInstance()
{
    if (m_pSkillRuneManager == nullptr)
        m_pSkillRuneManager = new SkillRuneManager();

    return m_pSkillRuneManager;
}

void SkillRuneManager::destroyInstance()
{
    if (m_pSkillRuneManager == nullptr)
        return;

    delete m_pSkillRuneManager;
    m_pSkillRuneManager = nullptr;
}

QString SkillRuneManager::getRuneIconPath(QString runeName)
{
    return QString(":/image/resources/runes/%1.png").arg(m_runeIndex[runeName]);
}
