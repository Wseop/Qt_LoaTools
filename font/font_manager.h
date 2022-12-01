#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QFont>
#include <QFontDatabase>

enum class FontFamily
{
    NanumSquareNeoLight,
    NanumSquareNeoRegular,
    NanumSquareNeoBold,
    NanumSquareNeoExtraBold,
    NanumSquareNeoHeavy,
    FontFamilySize
};

class FontManager
{
private:
    FontManager();
    ~FontManager();

    void loadFont();

public:
    static FontManager* getInstance();
    static void destroyInstance();

    QFont getFont(FontFamily fontFamily, int fontSize);

private:
    static FontManager* m_pFontManager;

    QStringList m_fontFamilies;
};

#endif // FONTMANAGER_H
