#include "loatools.h"
#include "db/db.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // run database
    QThread* dbThread = new QThread();
    DB::getInstance()->moveToThread(dbThread);
    dbThread->start();

    // run main widget
    LoaTools w;
    w.show();

    return a.exec();
}
