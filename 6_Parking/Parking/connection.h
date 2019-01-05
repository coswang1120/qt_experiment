#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QDebug>


static bool createConnection()
{
//    qDebug() << QSqlDatabase::drivers();//我们可以通过方法来获取当前的 sql 模快中支持哪些数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName("plumewind");
    db.setDatabaseName("vehicle.db");
    db.setUserName("admin");
    db.setPassword("admin");

    if(!db.open()){
        qDebug() << "Database Error!";
        return false;
    }

    QSqlQuery query;
    query.exec("create table vehicles(id varchar primary key,"
               "type varchar(20),brand varchar(20),"
               "liscene varchar(20),in_time varchar(20))");
//    query.exec("insert into vehicles values(0,'小轿车','大众','赣B1314','15:38:00')");
//    query.exec("insert into vehicles values(2,'大卡车','宝马','赣A2244')");
//    query.exec("insert into vehicles values(3,'摩托车','大众','赣B1214')");
//    query.exec("insert into vehicles values(4,'公交车','宝马','赣B2244')");

//    query.exec(QString("select * from vehicles"));
//    qDebug()<<"查询状态:"<<query.next();
//    qDebug()<<"车类型:"<<query.value("type").toString();
//    qDebug()<<"车品牌:"<<query.value("brand").toString();
//    qDebug()<<"车牌号:"<<query.value("liscene").toString();

//    query.exec("create table students(id varchar primary key,name varchar(20),age int)"); //id自动增加
//    query.exec("insert into students values(1,'小黑',14)");
//    query.exec("insert into students values(2,'小王',20)");

//    query.exec(QString("select * from students"));
//    qDebug()<<"查询状态:"<<query.next();
//    qDebug()<<"姓名:"<<query.value("name").toString();
//    qDebug()<<"年龄:"<<query.value("age").toInt();

    query.exec("create table carports(id int identity(1,1) primary key,stall int,remark varchar(20))");
//    query.exec("insert into carports(stall,remark) values(12,'freeports')");
//    query.exec(QString("select * from carports where remark='freeports'"));
//    qDebug()<<"查询状态:"<<query.next();
//    qDebug()<<"车位:"<<query.value("stall").toInt();

//    db.close();


    return true;
}
#endif // CONNECTION_H
