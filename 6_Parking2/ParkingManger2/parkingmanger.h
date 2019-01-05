#ifndef PARKINGMANGER_H
#define PARKINGMANGER_H

#include <QMainWindow>

namespace Ui {
class ParkingManger;
}

class ParkingManger : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParkingManger(QWidget *parent = 0);
    ~ParkingManger();

private:
    Ui::ParkingManger *ui;
};

#endif // PARKINGMANGER_H
