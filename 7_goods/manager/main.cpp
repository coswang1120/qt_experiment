#include "widget.h"
#include "connection.h"
#include "logindialog.h"
#include <QTextCodec>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 必须在创建数据库之前使用，不然无法在数据库中使用中文
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    if(!createConnection() || !createXml()) return 0;

    Widget w;
    LoginDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        w.show();
        return a.exec();
    } else {
        return 0;
    }
}

