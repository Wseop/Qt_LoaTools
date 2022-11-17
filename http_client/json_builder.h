#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <QString>
#include <QJsonDocument>

class JsonBuilder
{
public:
    static QJsonDocument buildCharacter(QString name, QString cls, double level, const QStringList& setEffects);
    static QJsonDocument buildAbility(QString name, QString cls, QStringList neck, QStringList ear, QStringList ring);
    static QJsonDocument buildEngrave(QString name, QString cls, QStringList engraves, QList<int> levels);
};

#endif // JSONBUILDER_H
