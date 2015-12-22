#ifndef RTTTIMER_H
#define RTTTIMER_H
#include<QTimer>
#include<QUdpSocket>
#include"commands.h"

class RttTimer : public QObject
{


    Q_OBJECT
public:
    RttTimer(int seq,QString ip, int port, QUdpSocket *udpSocket)
    {
        _ip = ip;
        _count = 0;
        _port = port;
        _timer = new QTimer();
        _udpSocket = udpSocket;
        _seq = seq;
        connect(_timer, SIGNAL(timeout()),
                 this, SLOT(timerSlot()));
    }
    void timerStart();
    void stopTimer() { _timer->stop();}
private:
        int _seq;
        QString _ip;
        int _port;
        int _count;
        QTimer *_timer;
        QUdpSocket *_udpSocket;

   public slots:
       void timerSlot();
};

#endif // RTTTIMER_H
