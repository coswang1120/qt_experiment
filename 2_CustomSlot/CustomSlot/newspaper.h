#ifndef NEWSPAPER_H
#define NEWSPAPER_H

#include <QObject>

class Newspaper : public QObject
{
    Q_OBJECT
public:
//    explicit Newspaper(QObject *parent = nullptr);
    Newspaper(const QString &name):
        m_name(name)
    {

    }

    void send()
    {
        emit newPaper(m_name);
    }

signals:
    void newPaper(const QString &name);

public :
    QString m_name;
};

#endif // NEWSPAPER_H
