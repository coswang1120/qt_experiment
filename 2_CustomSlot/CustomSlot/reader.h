#ifndef READER_H
#define READER_H

#include <QObject>
#include <QDebug>

class Reader : public QObject
{
    Q_OBJECT
public:
//    explicit reader(QObject *parent = nullptr);
    Reader(){}

    void receiveNewspaper(const QString &name)
    {
        qDebug()<<"Receive Newspaper:"<<name;
    }

//signals:

//public slots:
};

#endif // READER_H
