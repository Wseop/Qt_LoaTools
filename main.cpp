#include "loatools.h"
#include "db/db.h"
#include "http_client/http_client.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // run database
    QThread* dbThread = new QThread();
    DB::getInstance()->moveToThread(dbThread);
    dbThread->start();

    // run http_client
    QThread* httpThread = new QThread();
    HttpClient::getInstance()->moveToThread(httpThread);
    httpThread->start();

    // run main widget
    LoaTools w;
    w.show();

    return a.exec();
}
