#include "loatools.h"
#include "db/db.h"
#include "http_client/http_client.h"
#include "tools/accessory_searcher/auction_options.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // pre-load
    DB::getInstance();
    HttpClient::getInstance();
    AuctionOptions::getInstance();

    // run main widget
    LoaTools w;
    w.show();

    return a.exec();
}
