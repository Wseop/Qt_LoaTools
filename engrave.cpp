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
    QString filePath = QDir::currentPath() + "/resources/EngraveList.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << ": File open fail";
        return;
    }

    QTextStream inStream(&file);
    while (!inStream.atEnd())
        mEngraveList << inStream.readLine();
    file.close();

    mPenaltyList << "공격력 감소" << "공격속도 감소" << "방어력 감소" << "이동속도 감소";
}

QStringList Engrave::getEngraveList()
{
    return mEngraveList;
}

QStringList Engrave::getPenaltyList()
{
    return mPenaltyList;
}

bool Engrave::isValidEngrave(QString engrave)
{
    return mEngraveList.contains(engrave);
}

bool Engrave::isValidPenalty(QString penalty)
{
    return mPenaltyList.contains(penalty);
}

QString Engrave::getEngravePath(QString engrave)
{
    int index = mEngraveList.indexOf(engrave);
    return QString("%1/%2.png").arg(PATH_ENGRAVE).arg(index);
}

QString Engrave::getPenaltyPath(QString penalty)
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
