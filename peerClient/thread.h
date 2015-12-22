#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTcpSocket>
#include "commands.h"
class Thread : public QThread
{
     Q_OBJECT
private:
    QTcpSocket *_socket;
    qintptr _socketDescriptor;
public:

    Thread() {}
    Thread(qintptr ID, QObject *parent) :
        QThread(parent)
    {
        this->_socketDescriptor = ID;
    }

    ~Thread(){}
    void run();
signals:

public slots:
     void readyRead();
     void disconnected();
};

#endif // THREAD_H
