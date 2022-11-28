#include "loatools.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-aLt.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-bRg.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-cBd.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-dEb.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-eHv.ttf");

    LoaTools w;
    w.show();

    return a.exec();
}
