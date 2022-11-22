#include "engrave.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

Engrave::Engrave()
{
    loadEngraveList();
}

void Engrave::loadEngraveList()
{
    // 각인 리스트 초기화
    {
        QString filePath = QDir::currentPath() + "/resources/engrave_list.txt";
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << Q_FUNC_INFO << ": File open fail";
            return;
        }

        QTextStream inStream(&file);
        while (!inStream.atEnd())
            m_engraves << inStream.readLine();
        file.close();

        m_penalties << "공격력 감소" << "공격속도 감소" << "방어력 감소" << "이동속도 감소";
    }

    // 직업 각인 리스트 초기화
    {
        QString filePath = QDir::currentPath() + "/resources/class_engrave_list.txt";
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << Q_FUNC_INFO << ": File open fail";
            return;
        }

        QTextStream inStream(&file);
        while (!inStream.atEnd())
            m_classEngraves << inStream.readLine();
        file.close();
    }
}

const QStringList& Engrave::getEngraveList()
{
    return m_engraves;
}

const QStringList& Engrave::getPenaltyList()
{
    return m_penalties;
}

bool Engrave::isValidEngrave(QString engrave)
{
    return m_engraves.contains(engrave);
}

bool Engrave::isValidPenalty(QString penalty)
{
    return m_penalties.contains(penalty);
}

QString Engrave::getEngravePath(QString engrave) const
{
    int index = m_engraves.indexOf(engrave);
    return QString("%1/%2.png").arg(PATH_ENGRAVE).arg(index);
}

QString Engrave::getPenaltyPath(QString penalty) const
{
    if (penalty == "공격력 감소")
        return QString("%1/penalty_att.png").arg(PATH_ENGRAVE);
    else if (penalty == "공격속도 감소")
        return QString("%1/penalty_attspd.png").arg(PATH_ENGRAVE);
    else if (penalty == "방어력 감소")
        return QString("%1/penalty_def.png").arg(PATH_ENGRAVE);
    else if (penalty == "이동속도 감소")
        return QString("%1/penalty_spd.png").arg(PATH_ENGRAVE);
    else
        return "";
}

void Engrave::addEngrave(QString engrave, int value)
{
    m_engraveValue[engrave] += value;
}

void Engrave::addPenalty(QString penalty, int value)
{
    m_penaltyValue[penalty] += value;
}

QStringList Engrave::getActiveEngraveList() const
{
    QStringList ret;
    QStringList keys = m_engraveValue.keys();

    for (const QString& key : keys)
    {
        if (m_engraveValue[key] >= 5)
            ret << key;
    }
    return ret;
}

QStringList Engrave::getActivePenaltyList() const
{
    QStringList ret;
    QStringList keys = m_penaltyValue.keys();

    for (const QString& key : keys)
    {
        if (m_penaltyValue[key] >= 5)
            ret << key;
    }
    return ret;
}

int Engrave::getEngraveValue(QString engrave) const
{
    return m_engraveValue[engrave];
}

int Engrave::getPenaltyValue(QString penalty) const
{
    return m_penaltyValue[penalty];
}

void Engrave::clearEngrave()
{
    m_engraveValue.clear();
}

void Engrave::clearPenalty()
{
    m_penaltyValue.clear();
}

bool Engrave::isClassEngrave(QString engraveName)
{
    return m_classEngraves.contains(engraveName);
}

int Engrave::indexOf(QString engraveName)
{
    return m_engraves.indexOf(engraveName);
}

QString Engrave::getEngraveByIndex(int index)
{
    return m_engraves[index];
}
