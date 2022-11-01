#include "rune.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

Rune::Rune() : Item(Part::RUNE)
{
    initMap();
}

void Rune::setName(QString name)
{
    Item::setName(name);

    QString iconPath = QString(":/image/resources/runes/%1.png").arg(mRuneId[name]);
    setIconPath(iconPath);
}

void Rune::initMap()
{
    QString filePath = QDir::currentPath() + "/resources/rune_list.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << ": File open fail";
        return;
    }

    QTextStream inStream(&file);
    int id = 0;
    while (!inStream.atEnd())
        mRuneId[inStream.readLine()] = id++;

    file.close();
}

void Rune::setIconPath(QString iconPath)
{
    Item::setIconPath(iconPath);
}
