#ifndef PROFILE_H
#define PROFILE_H

#include "game_data/profile/enum/class.h"
#include "game_data/profile/enum/ability.h"

#include <QString>
#include <QMap>

class Profile
{
public:
    Profile();

    Class getClass() const;
    int getCharacterLevel() const;
    const QString& getCharacterName() const;
    const QString& getServer() const;
    const QString& getTitle() const;
    const QString& getGuild() const;
    int getExpLevel() const;
    double getItemLevel() const;
    const QMap<Ability, int>& getAbilities() const;

    void setClass(Class cls);
    void setCharacterLevel(int characterLevel);
    void setCharacterName(const QString& characterName);
    void setServer(const QString& server);
    void setTitle(const QString& title);
    void setGuild(const QString& guild);
    void setExpLevel(int expLevel);
    void setItemLevel(double itemLevel);
    void addAbility(Ability ability, int value);

private:
    Class m_class;
    int m_characterLevel;
    QString m_characterName;
    QString m_server;
    QString m_title;
    QString m_guild;
    int m_expLevel;
    double m_itemLevel;
    QMap<Ability, int> m_abilities;
};

#endif // PROFILE_H
