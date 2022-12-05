#include "engrave.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

Engrave* Engrave::m_pEngrave = nullptr;

Engrave::Engrave()
{
    loadEngraveList();
}

Engrave::~Engrave()
{
    destroyInstance();
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

Engrave *Engrave::getInstance()
{
    if (m_pEngrave == nullptr)
        m_pEngrave = new Engrave();

    return m_pEngrave;
}

void Engrave::destroyInstance()
{
    if (m_pEngrave == nullptr)
        return;

    delete m_pEngrave;
    m_pEngrave = nullptr;
}

QStringList Engrave::extractActiveEngraves(const QMap<QString, int> &engraveValues)
{
    QStringList result;

    const QStringList& engraves = engraveValues.keys();
    for (const QString& engrave : engraves)
    {
        if (engraveValues[engrave] >= 5)
            result << engrave;
    }

    return result;
}

QStringList Engrave::extractActivePenalties(const QMap<QString, int> &penaltyValues)
{
    QStringList result;

    const QStringList& penalties = penaltyValues.keys();
    for (const QString& penalty : penalties)
    {
        if (penaltyValues[penalty] >= 5)
            result << penalty;
    }

    return result;
}

const QStringList& Engrave::getEngraveList()
{
    return m_engraves;
}

const QStringList& Engrave::getPenaltyList()
{
    return m_penalties;
}

QString Engrave::getEngravePath(QString engrave) const
{
    int index = m_engraves.indexOf(engrave);
    return QString(":/image/resources/engraves/%1.png").arg(index);
}

QString Engrave::getPenaltyPath(QString penalty) const
{
    if (penalty == "공격력 감소")
        return QString(":/image/resources/engraves/penalty_att.png");
    else if (penalty == "공격속도 감소")
        return QString(":/image/resources/engraves/penalty_attspd.png");
    else if (penalty == "방어력 감소")
        return QString(":/image/resources/engraves/penalty_def.png");
    else if (penalty == "이동속도 감소")
        return QString(":/image/resources/engraves/penalty_spd.png");
    else
        return "";
}

bool Engrave::isValidEngrave(QString engrave)
{
    return m_engraves.contains(engrave);
}

bool Engrave::isValidPenalty(QString penalty)
{
    return m_penalties.contains(penalty);
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
