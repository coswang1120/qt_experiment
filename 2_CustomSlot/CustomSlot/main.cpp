#include "mainwindow.h"
#include <QApplication>
#include "newspaper.h"
#include "reader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    Newspaper newspaper("Newspaper A");
    Reader reader;
    QObject::connect(&newspaper, &Newspaper::newPaper,
                     &reader,    &Reader::receiveNewspaper);
    newspaper.send();

    return a.exec();
}
