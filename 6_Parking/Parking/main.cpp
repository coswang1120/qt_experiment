#include "parkingmanger.h"
#include "connection.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    if(!createConnection()) return 0;
    ParkingManger w;
    w.show();

    return a.exec();
}
