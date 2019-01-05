#include "athletematch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AthleteMatch w;
    w.show();

    return a.exec();
}
