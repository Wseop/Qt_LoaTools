#include "loatools.h"
#include "db/db.h"

#include <QApplication>
#include <QFontDatabase>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-aLt.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-bRg.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-cBd.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-dEb.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/fonts/NanumSquareNeo-eHv.ttf");

    QThread* dbThread = new QThread();
    DB::getInstance()->moveToThread(dbThread);
    dbThread->start();

    LoaTools w;
    w.show();

    return a.exec();
}
