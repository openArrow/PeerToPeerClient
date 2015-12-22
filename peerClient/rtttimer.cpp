#include "rtttimer.h"
#include"mainwindow.h"

void RttTimer::timerSlot()
{
    _count++;;
    char buff[128];
    if(_count == 10)
        _timer->stop();
    qDebug() << "timeout for ACK "+ QString::number(_seq);
    if(!MainWindow::getAckSeqList().contains(_seq) &&
       MainWindow::getDataToserverMap().contains(_seq) )
    {
        CMD::FileInfo fileInfo = MainWindow::getDataToserverMap().value(_seq);
        fileInfo.serialize(buff);
        _udpSocket->writeDatagram(buff,50,QHostAddress(QString::fromStdString(fileInfo.getIp())),
                                  fileInfo.getPort());
    }
    else if(MainWindow::getAckSeqList().contains(_seq))
    _timer->stop();

}
void RttTimer::timerStart()
{
    _timer->start(MainWindow::getTimerValue());
}
