#include "rtttimer.h"
#include "udpserver.h"
void RttTimer::timerSlot()
{
    char buff[128];
    _count++;
    if(_count == 10)
        _timer->stop();
    qDebug() << "timeout for ACK "+ QString::number(_seq) + " for Ip:" + _ip;
    QString key = QString::number(_seq) + ":" + _ip;
    if(!UdpServer::getAckClienMap().contains(key) &&
       UdpServer::getClientData().contains(key) )
    {
        RSP::QueryOutput queryOutput = UdpServer::getClientData().value(key);
        queryOutput.serialize(buff);
        _udpSocket->writeDatagram(buff,50,QHostAddress(_ip),
                                  _port);
    }
    else if(UdpServer::getAckClienMap().contains(key))
    _timer->stop();

}
void RttTimer::timerStart()
{
    _timer->start(UdpServer::getTimerValue());
}
