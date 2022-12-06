#ifndef CLASS_H
#define CLASS_H

#include <QString>

class Class
{
public:
    enum {
        None,
        Destroyer,
        Warlord,
        Berserker,
        Holyknight,
        Striker,
        Battlemaster,
        Infighter,
        Soulmaster,
        Lancemaster,
        DevilHunter,
        Blaster,
        Hawkeye,
        Scouter,
        Gunslinger,
        Bard,
        Summoner,
        Arcana,
        Sorceress,
        Blade,
        Demonic,
        Reaper,
        Artist,
        Aeromancer
    };

    static inline int start()
    {
        return Class::Destroyer;
    }

    static inline int size()
    {
        // None을 제외한 직업의 갯수를 반환
        return Class::Aeromancer;
    }

    static inline QString enumToKstr(int cls)
    {
        switch (cls)
        {
        case Class::Destroyer:
            return "디스트로이어";
        case Class::Warlord:
            return "워로드";
        case Class::Berserker:
            return "버서커";
        case Class::Holyknight:
            return "홀리나이트";
        case Class::Striker:
            return "스트라이커";
        case Class::Battlemaster:
            return "배틀마스터";
        case Class::Infighter:
            return "인파이터";
        case Class::Soulmaster:
            return "기공사";
        case Class::Lancemaster:
            return "창술사";
        case Class::DevilHunter:
            return "데빌헌터";
        case Class::Blaster:
            return "블래스터";
        case Class::Hawkeye:
            return "호크아이";
        case Class::Scouter:
            return "스카우터";
        case Class::Gunslinger:
            return "건슬링어";
        case Class::Bard:
            return "바드";
        case Class::Summoner:
            return "서머너";
        case Class::Arcana:
            return "아르카나";
        case Class::Sorceress:
            return "소서리스";
        case Class::Blade:
            return "블레이드";
        case Class::Demonic:
            return "데모닉";
        case Class::Reaper:
            return "리퍼";
        case Class::Artist:
            return "도화가";
        case Class::Aeromancer:
            return "기상술사";
        default:
            return "";
        }
    }

    static inline int kStrToEnum(QString cls)
    {
        if (cls == "디스트로이어")
            return Class::Destroyer;
        else if (cls == "워로드")
            return Class::Warlord;
        else if (cls == "버서커")
            return Class::Berserker;
        else if (cls == "홀리나이트")
            return Class::Holyknight;
        else if (cls == "스트라이커")
            return Class::Striker;
        else if (cls == "배틀마스터")
            return Class::Battlemaster;
        else if (cls == "인파이터")
            return Class::Infighter;
        else if (cls == "기공사")
            return Class::Soulmaster;
        else if (cls == "창술사")
            return Class::Lancemaster;
        else if (cls == "데빌헌터")
            return Class::DevilHunter;
        else if (cls == "블래스터")
            return Class::Blaster;
        else if (cls == "호크아이")
            return Class::Hawkeye;
        else if (cls == "스카우터")
            return Class::Scouter;
        else if (cls == "건슬링어")
            return Class::Gunslinger;
        else if (cls == "바드")
            return Class::Bard;
        else if (cls == "서머너")
            return Class::Summoner;
        else if (cls == "아르카나")
            return Class::Arcana;
        else if (cls == "소서리스")
            return Class::Sorceress;
        else if (cls == "블레이드")
            return Class::Blade;
        else if (cls == "데모닉")
            return Class::Demonic;
        else if (cls == "리퍼")
            return Class::Reaper;
        else if (cls == "도화가")
            return Class::Artist;
        else if (cls == "기상술사")
            return Class::Aeromancer;
        else
            return Class::None;
    }

    static inline QString enumToEstr(int cls)
    {
        switch (cls)
        {
        case Class::Destroyer:
            return "Destroyer";
        case Class::Warlord:
            return "Warlord";
        case Class::Berserker:
            return "Berserker";
        case Class::Holyknight:
            return "Holyknight";
        case Class::Striker:
            return "Striker";
        case Class::Battlemaster:
            return "Battlemaster";
        case Class::Infighter:
            return "Infighter";
        case Class::Soulmaster:
            return "Soulmaster";
        case Class::Lancemaster:
            return "Lancemaster";
        case Class::DevilHunter:
            return "DevilHunter";
        case Class::Blaster:
            return "Blaster";
        case Class::Hawkeye:
            return "Hawkeye";
        case Class::Scouter:
            return "Scouter";
        case Class::Gunslinger:
            return "Gunslinger";
        case Class::Bard:
            return "Bard";
        case Class::Summoner:
            return "Summoner";
        case Class::Arcana:
            return "Arcana";
        case Class::Sorceress:
            return "Sorceress";
        case Class::Blade:
            return "Blade";
        case Class::Demonic:
            return "Demonic";
        case Class::Reaper:
            return "Reaper";
        case Class::Artist:
            return "Artist";
        case Class::Aeromancer:
            return "Aeromancer";
        default:
            return "";
        }
    }

    static inline int eStrToEnum(QString cls)
    {
        if (cls == "Destroyer")
            return Class::Destroyer;
        else if (cls == "Warlord")
            return Class::Warlord;
        else if (cls == "Berserker")
            return Class::Berserker;
        else if (cls == "Holyknight")
            return Class::Holyknight;
        else if (cls == "Striker")
            return Class::Striker;
        else if (cls == "Battlemaster")
            return Class::Battlemaster;
        else if (cls == "Infighter")
            return Class::Infighter;
        else if (cls == "Soulmaster")
            return Class::Soulmaster;
        else if (cls == "Lancemaster")
            return Class::Lancemaster;
        else if (cls == "DevilHunter")
            return Class::DevilHunter;
        else if (cls == "Blaster")
            return Class::Blaster;
        else if (cls == "Hawkeye")
            return Class::Hawkeye;
        else if (cls == "Scouter")
            return Class::Scouter;
        else if (cls == "Gunslinger")
            return Class::Gunslinger;
        else if (cls == "Bard")
            return Class::Bard;
        else if (cls == "Summoner")
            return Class::Summoner;
        else if (cls == "Arcana")
            return Class::Arcana;
        else if (cls == "Sorceress")
            return Class::Sorceress;
        else if (cls == "Blade")
            return Class::Blade;
        else if (cls == "Demonic")
            return Class::Demonic;
        else if (cls == "Reaper")
            return Class::Reaper;
        else if (cls == "Artist")
            return Class::Artist;
        else if (cls == "Aeromancer")
            return Class::Aeromancer;
        else
            return Class::None;
    }

    static inline QString kStrToEstr(QString cls)
    {
        if (cls == "디스트로이어")
            return "Destroyer";
        else if (cls == "워로드")
            return "Warlord";
        else if (cls == "버서커")
            return "Berserker";
        else if (cls == "홀리나이트")
            return "Holyknight";
        else if (cls == "스트라이커")
            return "Striker";
        else if (cls == "배틀마스터")
            return "Battlemaster";
        else if (cls == "인파이터")
            return "Infighter";
        else if (cls == "기공사")
            return "Soulmaster";
        else if (cls == "창술사")
            return "Lancemaster";
        else if (cls == "데빌헌터")
            return "DevilHunter";
        else if (cls == "블래스터")
            return "Blaster";
        else if (cls == "호크아이")
            return "Hawkeye";
        else if (cls == "스카우터")
            return "Scouter";
        else if (cls == "건슬링어")
            return "Gunslinger";
        else if (cls == "바드")
            return "Bard";
        else if (cls == "서머너")
            return "Summoner";
        else if (cls == "아르카나")
            return "Arcana";
        else if (cls == "소서리스")
            return "Sorceress";
        else if (cls == "블레이드")
            return "Blade";
        else if (cls == "데모닉")
            return "Demonic";
        else if (cls == "리퍼")
            return "Reaper";
        else if (cls == "도화가")
            return "Artist";
        else if (cls == "기상술사")
            return "Aeromancer";
        else
            return "";
    }

    static inline QString eStrToKstr(QString cls)
    {
        if (cls == "Destroyer")
            return "디스트로이어";
        else if (cls == "Warlord")
            return "워로드";
        else if (cls == "Berserker")
            return "버서커";
        else if (cls == "Holyknight")
            return "홀리나이트";
        else if (cls == "Striker")
            return "스트라이커";
        else if (cls == "Battlemaster")
            return "배틀마스터";
        else if (cls == "Infighter")
            return "인파이터";
        else if (cls == "Soulmaster")
            return "기공사";
        else if (cls == "Lancemaster")
            return "창술사";
        else if (cls == "DevilHunter")
            return "데빌헌터";
        else if (cls == "Blaster")
            return "블래스터";
        else if (cls == "Hawkeye")
            return "호크아이";
        else if (cls == "Scouter")
            return "스카우터";
        else if (cls == "Gunslinger")
            return "건슬링어";
        else if (cls == "Bard")
            return "바드";
        else if (cls == "Summoner")
            return "서머너";
        else if (cls == "Arcana")
            return "아르카나";
        else if (cls == "Sorceress")
            return "소서리스";
        else if (cls == "Blade")
            return "블레이드";
        else if (cls == "Demonic")
            return "데모닉";
        else if (cls == "Reaper")
            return "리퍼";
        else if (cls == "Artist")
            return "도화가";
        else if (cls == "Aeromancer")
            return "기상술사";
        else
            return "";
    }
};

#endif // CLASS_H
