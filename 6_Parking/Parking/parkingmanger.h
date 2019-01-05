#ifndef PARKINGMANGER_H
#define PARKINGMANGER_H

#include <QWidget>

namespace Ui {
class ParkingManger;
}

class ParkingManger : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingManger(QWidget *parent = 0);
    ~ParkingManger();

    enum DateTimeType{Time,Date,DateTime};
    QString getDateTime(DateTimeType type);

private slots:
    void on_button_car_in_clicked();

    void on_button_car_all_clicked();

    void on_button_car_inquire_clicked();

    void on_button_car_out_clicked();

private:
    Ui::ParkingManger *ui;
    int VehicleNumber;
    int VehiclePrice;
    int FreePortNumber;
};

#endif // PARKINGMANGER_H
