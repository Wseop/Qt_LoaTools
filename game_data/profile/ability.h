#ifndef ABILITY_H
#define ABILITY_H

#include <QString>

class Ability
{
public:
    enum
    {
        Critical,
        Specialization,
        Swift,
        Domination,
        Endurance,
        Expertise,
    };

    static inline int size()
    {
        return static_cast<int>(Ability::Expertise) + 1;
    }

    static inline QString enumToKStr(int ability)
    {
        switch (ability)
        {
        case Critical:
            return "치명";
        case Specialization:
            return "특화";
        case Swift:
            return "신속";
        case Domination:
            return "제압";
        case Endurance:
            return "인내";
        case Expertise:
            return "숙련";
        default:
            return "";
        }
    }

    static inline int kstrToEnum(QString ability)
    {
        if ("치명" == ability)
            return Critical;
        else if ("특화" == ability)
            return Specialization;
        else if ("신속" == ability)
            return Swift;
        else if ("제압" == ability)
            return Domination;
        else if ("인내" == ability)
            return Endurance;
        else if ("숙련" == ability)
            return Expertise;
        else
            return -1;
    }
};

#endif // ABILITY_H
