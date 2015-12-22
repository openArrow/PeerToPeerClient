#ifndef UDPSERVER_H
#define UDPSERVER_H
#include<QUdpSocket>
#include<QHash>
#include<QPair>
#include "commands.h"
#include "rtttimer.h"
class UdpServer : public QObject
{
    Q_OBJECT
private:
    QMultiHash <QString, QPair<QString,int> > _fileNameClientMap;
    QList<QString> _clientList;
    QUdpSocket *_udpSocket;
    int _serverPort;
    int _seqNo;
    static QMap<QString,RttTimer*> _timerAckMap;
    static QMap<QString, RSP::QueryOutput> _clientData; //key is ip:sequence number
    static QList<QString> _ackClienMap; //key as seq:ip
    void readConfFile();

public:

    UdpServer()
    {
        _seqNo = 0;
        _serverPort = 8989;
        readConfFile();
        _udpSocket = new QUdpSocket();
        _udpSocket->bind(QHostAddress::AnyIPv4, _serverPort);
        connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    static QMap<QString,RttTimer*> getTimerAckMap() { return _timerAckMap; }
    static QMap<QString, RSP::QueryOutput> getClientData() { return _clientData; }
    static QList<QString> getAckClienMap() { return _ackClienMap; }
    static getTimerValue() { return 1000;}
    ~UdpServer(){}

    signals:

    public slots:
      void readyRead();

};

#endif // UDPSERVER_H
