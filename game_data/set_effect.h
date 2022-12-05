#ifndef SET_EFFECT_H
#define SET_EFFECT_H

#include <QString>

class SetEffect
{
public:
    enum
    {
        악몽,
        구원,
        사멸,
        지배,
        환각,
        갈망,
        배신,
        매혹,
        파괴
    };

    static inline int size()
    {
        return static_cast<int>(SetEffect::파괴) + 1;
    }

    static inline QString enumToStr(int setEffect)
    {
        switch (setEffect)
        {
        case 구원:
            return "구원";
        case 지배:
            return "지배";
        case 악몽:
            return "악몽";
        case 사멸:
            return "사멸";
        case 환각:
            return "환각";
        case 갈망:
            return "갈망";
        case 배신:
            return "배신";
        case 매혹:
            return "매혹";
        case 파괴:
            return "파괴";
        default:
            return "";
        }
    }

    static inline int strToEnum(QString setEffect)
    {
        if ("구원" == setEffect)
            return 구원;
        else if ("지배" == setEffect)
            return 지배;
        else if ("악몽" == setEffect)
            return 악몽;
        else if ("사멸" == setEffect)
            return 사멸;
        else if ("환각" == setEffect)
            return 환각;
        else if ("갈망" == setEffect)
            return 갈망;
        else if ("배신" == setEffect)
            return 배신;
        else if ("매혹" == setEffect)
            return 매혹;
        else if ("파괴" == setEffect)
            return 파괴;
        else
            return -1;
    }
};

#endif // SET_EFFECT_H
