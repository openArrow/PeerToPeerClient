#ifndef THREADDOWNLOADER_H
#define THREADDOWNLOADER_H

#include <QThread>
#include <QTcpSocket>
#include "commands.h"

class ThreadDownloader :public QThread
{
    Q_OBJECT
private:
   QTcpSocket *_socket;
   STRING _filename;
   STRING _ip;
   SIGNED_INT _port;
   QHash<int, QString> _dataMap;
   int _numOfpackets;

public:
    ThreadDownloader(STRING filename,STRING ip,SIGNED_INT port,
                     STRING sendIp,SIGNED_INT sendport,
                     QObject *parent):QThread(parent)
    {
        _numOfpackets = 0;
        _filename = filename;
        _ip = ip;
        _port = port;
        _socket = new QTcpSocket();
        _socket->connectToHost(QString::fromStdString(sendIp),sendport);
        _socket->waitForConnected(5000);
        char buff[128];
        CMD::ClientRequest clientRequest(filename, "maveric",_ip,_port);
        int bytes = clientRequest.serialize(buff);
        if(_socket->state() != QAbstractSocket::UnconnectedState)
        {
        _socket->write(buff, bytes); //write size of data
        _socket->waitForBytesWritten();
        }
}

    ~ThreadDownloader(){}
    void run();
private:
    void writeToFile();
signals:
    updateDownloaderModel(QString filename);
public slots:
    void readyRead();
    void disconnected();
};

#endif // THREADDOWNLOADER_H
