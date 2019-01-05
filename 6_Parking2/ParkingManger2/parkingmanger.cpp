#include "parkingmanger.h"
#include "ui_parkingmanger.h"

ParkingManger::ParkingManger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParkingManger)
{
    ui->setupUi(this);
}

ParkingManger::~ParkingManger()
{
    delete ui;
}
