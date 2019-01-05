#include "parkingmanger.h"
#include "ui_parkingmanger.h"
#include "connection.h"
#include <QtSql>
#include <QtGui>
//#include <QSplitter>
#include <QDebug>
#include <QMessageBox>

ParkingManger::ParkingManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingManger)
{
    ui->setupUi(this);
//    setFixedSize(750,600);
    ui->stackedWidget->setCurrentIndex(0);

//    QSplitter *splitter = new QSplitter(ui->managePage);
//    splitter->resize(700, 500);
//    splitter->move(0, 50);

//    splitter->addWidget(ui->toolBox);
//    splitter->addWidget(ui->list_view);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);
//    on_button_car_all_clicked();
    VehicleNumber=0;
    FreePortNumber=0;
    VehiclePrice=1;//停车单价为0.01元每秒
    QSqlQuery query;
    query.exec(QString("select * from carports"));
    while(query.next())//初始化车辆总数和停车单价
    {
        if(query.value("remark").toString()=="VehicleNumber")
            VehicleNumber=query.value("stall").toInt();
        qDebug() <<VehiclePrice;
        if(query.value("remark").toString()=="VehiclePrice")
            VehiclePrice=query.value("stall").toInt();
        if(query.value("remark").toString()=="FreePortNumber")
            FreePortNumber=query.value("stall").toInt();
    }
//    qDebug() <<"price0:"<<VehiclePrice;
}

ParkingManger::~ParkingManger()
{
    QSqlQuery query;
    bool first1=false;//车辆总数更新标志位
    bool first2=false;//停车单价更新标志位
    bool first3=false;//临时车库数量更新标志位
    //更新车辆总数及单价
    query.exec("select * from carports");
    while(query.next())//初始化数据库里车辆总数，第一次则插入条目
    {
        if(query.value("remark").toString()=="VehicleNumber")
            first1=true;
        if(query.value("remark").toString()=="VehiclePrice")
            first2=true;
        if(query.value("remark").toString()=="FreePortNumber")
            first3=true;
    }

    if(!first1) //第一次储存车辆总数,则插入条目
        query.exec(QString("insert into carports(stall,remark) values(%1,'VehicleNumber')").arg(VehicleNumber));
    else query.exec(QString("update carports set stall=%1 where remark='VehicleNumber'").arg(VehicleNumber));
    if(!first2) //第一次储存消费单价,则插入条目
        query.exec(QString("insert into carports(stall,remark) values(%1,'VehiclePrice')").arg(VehiclePrice));
    else query.exec(QString("update carports set stall=%1 where remark='VehiclePrice'").arg(VehiclePrice));
//    qDebug()<<"price0:"  <<VehiclePrice;
    if(!first3) //第一次储存临时车库总数数目,则插入条目
        query.exec(QString("insert into carports(stall,remark) values(%1,'FreePortNumber')").arg(FreePortNumber));
    else query.exec(QString("update carports set stall=%1 where remark='FreePortNumber'").arg(FreePortNumber));
//    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//    db.close();
    delete ui;
}

QString ParkingManger::getDateTime(ParkingManger::DateTimeType type)
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString date=datetime.toString("yyyy-MM-dd");
    QString time=datetime.toString("hh:mm");
    QString dateAndTime=datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type==Date) return date;
    else if(type==Time) return time;
    else return dateAndTime;
}

void ParkingManger::on_button_car_in_clicked()
{
    int car_port=0;
    int car_newport=0;
    QString car_type=ui->cartype_in->currentText();
    QString car_brand=ui->carbrand_in->currentText();
    QString car_liscene=ui->carnumber_in->text();

    QListWidgetItem *message=new QListWidgetItem;
    QString currentdate=getDateTime(DateTime);

    QSqlQuery query;//执行数据插入操作
    query.exec(QString("select * from vehicles"));
    while(query.next())//重复车牌号不准进入
    {
        if(query.value("liscene").toString()==car_liscene)
        {
            QMessageBox::warning(this,tr("错误提示"),tr("已有该车牌号的车辆入库！"),QMessageBox::Ok);
            return ;
        }
        if(query.value("id").toInt()>car_newport)
            car_newport=query.value("id").toInt();//存储最大车位数
    }
    //先检查出库下的空闲车位，再去安排新车位
    query.exec(QString("select * from carports where remark='freeports'"));
    if(query.next())//有车位的话，使用之前的车位
    {
        car_port=query.value("stall").toInt();
        //删除该车位的所有信息
        query.prepare("delete from carports where remark='freeports' and stall=:stall");
        query.bindValue(":stall",car_port);
        query.exec();
        FreePortNumber--;
    }else{//没有的话，给安排新车位
        car_port=car_newport+1;
    }
    //准备入库操作
    query.prepare("insert into vehicles(id,type,brand,liscene,in_time) "
               "values(:id,:type,:brand,:liscene,:in_time)");
    query.bindValue(":id",car_port);
    query.bindValue(":type",car_type);
    query.bindValue(":brand",car_brand);
    query.bindValue(":liscene",car_liscene);
    query.bindValue(":in_time",currentdate);
    query.exec();
    VehicleNumber++;//记录当前入库车辆数，加1
    //窗口显示插入信息
    message->setText(QString("=========!!!%1!!!=========").arg(QString("车辆驶入")));
    message->setTextAlignment(Qt::AlignCenter);
    ui->list_view->addItem(message);
    ui->list_view->addItem(QString("入库时间：%1").arg(currentdate));
    ui->list_view->addItem(QString("一辆车牌号为%1的%2系%3已经入库！")
                           .arg(car_liscene).arg(car_brand).arg(car_type));

}

void ParkingManger::on_button_car_all_clicked()
{
    QListWidgetItem *message=new QListWidgetItem;
    message->setText(QString("=========!!!%1!!!=========").arg(QString("车辆查询")));
    message->setTextAlignment(Qt::AlignCenter);
    ui->list_view->addItem(message);
    ui->list_view->addItem(QString("库内车辆共有：%1辆").arg(VehicleNumber));
//    qDebug() <<QDateTime::currentDateTime().toTime_t();

    QSqlQuery query;
    query.exec(QString("select * from vehicles"));
//    qDebug() <<query.size();
    while(query.next())
    {
        ui->list_view->addItem(QString("占%1号车位,入库时间：%2\n\t车牌号：%3--车辆类型：%4--车辆品牌：%5")
                               .arg(query.value("id").toInt())
                               .arg(query.value("in_time").toString())
                               .arg(query.value("liscene").toString())
                               .arg(query.value("type").toString())
                               .arg(query.value("brand").toString()));
    }
    ui->list_view->addItem("车辆入库情况查询结束！");
    ui->list_view->addItem(QString("临时车位有：%1个").arg(FreePortNumber));
    query.exec(QString("select * from carports where remark='freeports'"));
    while(query.next())
    {
        ui->list_view->addItem(QString("%1号车位").arg(query.value("stall").toInt()));
    }
    ui->list_view->addItem("临时车库情况查询结束！");
}

void ParkingManger::on_button_car_inquire_clicked()
{
    double price=((double)VehiclePrice)/100.00;
    QString fowarddate;
    int date1,date2;
    QString car_type=ui->cartype_inquire->currentText();
    QString car_brand=ui->carbrand_inquire->currentText();
    QString car_liscene=ui->carnumber_inquire->text();

    QListWidgetItem *message=new QListWidgetItem;
    QSqlQuery query;
    query.exec(QString("select * from vehicles"));
    while(query.next())
    {
        if((query.value("liscene").toString()==car_liscene)&&
           (query.value("type").toString()==car_type)&&
           (query.value("brand").toString()==car_brand))
        {
            message->setText(QString("=========!!!%1!!!=========").arg(QString("车辆查询")));
            message->setTextAlignment(Qt::AlignCenter);
            ui->list_view->addItem(message);
            ui->list_view->addItem(QString("在%1号车位,入库时间：%2\n\t车牌号：%3--车辆类型：%4--车辆品牌：%5")
                                   .arg(query.value("id").toInt())
                                   .arg(query.value("in_time").toString())
                                   .arg(query.value("liscene").toString())
                                   .arg(query.value("type").toString())
                                   .arg(query.value("brand").toString()));
            fowarddate=query.value("in_time").toString();
            date1=QDateTime::fromString(fowarddate, "yyyy-MM-dd dddd hh:mm").toTime_t();
            date2=QDateTime::currentDateTime().toTime_t();
//            qDebug()<<"price:" <<price;
            ui->list_view->addItem(QString("该车停库已经有%1秒，现在花费%2元！").arg(date2-date1).arg((date2-date1)*price));
            return ;
        }
    }
    QMessageBox::warning(this,tr("错误提示"),tr("本车库内没有该车辆！"),QMessageBox::Ok);
}

void ParkingManger::on_button_car_out_clicked()
{
    int car_deleport=0;
    double price=((double)VehiclePrice)/100.00;
    QMessageBox::StandardButton rb;
    QListWidgetItem *message=new QListWidgetItem;
    QString car_liscene=ui->carnumber_out->text();
    QString fowarddate;
    int date1,date2;
    QSqlQuery query;
    query.exec(QString("select * from vehicles"));
    while(query.next())
    {
        if((query.value("liscene").toString()==car_liscene))
        {
            fowarddate=query.value("in_time").toString();
            car_deleport=query.value("id").toInt();

            date1=QDateTime::fromString(fowarddate, "yyyy-MM-dd dddd hh:mm").toTime_t();
            date2=QDateTime::currentDateTime().toTime_t();

            rb = QMessageBox::question(this, "提醒", QString("该车停库%1秒，是否出库?").arg(date2-date1), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            break;
        }
    }

    if(rb == QMessageBox::Yes)
    {
        message->setText(QString("=========!!!%1!!!=========").arg(QString("车辆驶出")));
        message->setTextAlignment(Qt::AlignCenter);
        ui->list_view->addItem(message);
//        qDebug()<<"price:" <<price;
        ui->list_view->addItem(QString("在第%1号车位,入库时间：%2\n\t车牌号：%3--车辆类型：%4--车辆品牌：%5")
                               .arg(car_deleport)
                               .arg(query.value("in_time").toString())
                               .arg(query.value("liscene").toString())
                               .arg(query.value("type").toString())
                               .arg(query.value("brand").toString()));
        ui->list_view->addItem(QString("该车停库%1秒，总共花费%2元！").arg(date2-date1).arg((date2-date1)*price));
        //储存出库车辆的车位
//        query.exec("insert into carports(stall,remark) values(12,'freeports')");
        query.prepare("insert into carports(stall,remark) values(:stall,:remark)");
        query.bindValue(":stall",car_deleport);
        query.bindValue(":remark","freeports");
        query.exec();
        FreePortNumber++;
//        qDebug()<<car_deleport;
        //删除该车的所有信息
        query.prepare("delete from vehicles where liscene=:liscene");
        query.bindValue(":liscene", car_liscene);
        query.exec();
        VehicleNumber--;//记录当前入库车辆数，减1
        return ;
    }
    QMessageBox::warning(this,tr("错误提示"),tr("车库内没有该车牌号的车辆入库！"),QMessageBox::Ok);
}
