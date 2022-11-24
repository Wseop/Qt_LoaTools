#ifndef CLASS_H
#define CLASS_H

#include <QString>

enum class Class
{
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

static int getNumOfClass()
{
    return static_cast<int>(Class::Aeromancer);
}

static Class strToEnumClass(QString str)
{
    if (str == "Destroyer")
        return Class::Destroyer;
    else if (str == "Warlord")
        return Class::Warlord;
    else if (str == "Berserker")
        return Class::Berserker;
    else if (str == "Holyknight")
        return Class::Holyknight;
    else if (str == "Striker")
        return Class::Striker;
    else if (str == "Battlemaster")
        return Class::Battlemaster;
    else if (str == "Infighter")
        return Class::Infighter;
    else if (str == "Soulmaster")
        return Class::Soulmaster;
    else if (str == "Lancemaster")
        return Class::Lancemaster;
    else if (str == "DevilHunter")
        return Class::DevilHunter;
    else if (str == "Blaster")
        return Class::Blaster;
    else if (str == "Hawkeye")
        return Class::Hawkeye;
    else if (str == "Scouter")
        return Class::Scouter;
    else if (str == "Gunslinger")
        return Class::Gunslinger;
    else if (str == "Bard")
        return Class::Bard;
    else if (str == "Summoner")
        return Class::Summoner;
    else if (str == "Arcana")
        return Class::Arcana;
    else if (str == "Sorceress")
        return Class::Sorceress;
    else if (str == "Blade")
        return Class::Blade;
    else if (str == "Demonic")
        return Class::Demonic;
    else if (str == "Reaper")
        return Class::Reaper;
    else if (str == "Artist")
        return Class::Artist;
    else if (str == "Aeromancer")
        return Class::Aeromancer;
    else
        return Class::None;
}

static QString enumClassToStr(Class cls)
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
        return "None";
    }
}

static QString enumClassToKStr(Class cls)
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

static QString enumClassKtoE(QString str)
{
    if (str == "디스트로이어")
        return "Destroyer";
    else if (str == "워로드")
        return "Warlord";
    else if (str == "버서커")
        return "Berserker";
    else if (str == "홀리나이트")
        return "Holyknight";
    else if (str == "스트라이커")
        return "Striker";
    else if (str == "배틀마스터")
        return "Battlemaster";
    else if (str == "인파이터")
        return "Infighter";
    else if (str == "기공사")
        return "Soulmaster";
    else if (str == "창술사")
        return "Lancemaster";
    else if (str == "데빌헌터")
        return "DevilHunter";
    else if (str == "블래스터")
        return "Blaster";
    else if (str == "호크아이")
        return "Hawkeye";
    else if (str == "스카우터")
        return "Scouter";
    else if (str == "건슬링어")
        return "Gunslinger";
    else if (str == "바드")
        return "Bard";
    else if (str == "서머너")
        return "Summoner";
    else if (str == "아르카나")
        return "Arcana";
    else if (str == "소서리스")
        return "Sorceress";
    else if (str == "블레이드")
        return "Blade";
    else if (str == "데모닉")
        return "Demonic";
    else if (str == "리퍼")
        return "Reaper";
    else if (str == "도화가")
        return "Artist";
    else if (str == "기상술사")
        return "Aeromancer";
    else
        return "None";
}

#endif // CLASS_H
