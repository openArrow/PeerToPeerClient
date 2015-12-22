#ifndef RTTTIMER_H
#define RTTTIMER_H
#include<QTimer>
#include<QUdpSocket>
#include"commands.h"

class RttTimer : public QObject
{


    Q_OBJECT
public:
    RttTimer(int seq,QUdpSocket *udpSocket)
    {
        _count = 0;

        _timer = new QTimer();
        _udpSocket = udpSocket;
        _seq = seq;
        connect(_timer, SIGNAL(timeout()),
                 this, SLOT(timerSlot()));
    }
    int getTimeElapsed()
    {
        return (_timer->interval() - _timer->remainingTime());
    }
    void timerStart();
    void stopTimer() { _timer->stop();}
private:
        int _seq;
        int _count;
        QTimer *_timer;
        QUdpSocket *_udpSocket;

   public slots:
       void timerSlot();
};

#endif // RTTTIMER_H
