#include "udpserver.h"
#include "commands.h"
#include <QFile>
int udpDataSize=50;
QMap<QString,RttTimer*> UdpServer::_timerAckMap;
QMap<QString, RSP::QueryOutput> UdpServer::_clientData; //key is ip:sequence number
QList<QString> UdpServer::_ackClienMap;
void UdpServer::readConfFile()
{
    std::cout<<"ENTER PORT FOR THE SERVER:\n";
    std::cin>>_serverPort;
    cout<<"\nServer Started\n";
//    QFile settingFile("C:\\Users\\maveric\\Documents\\build-peerServer-Desktop_Qt_5_5_0_MinGW_32bit-Debug\\debug\\settings.conf");
//    settingFile.open(QIODevice::ReadOnly);
// /*   if(!settingFile.isOpen())
//        std::cout <<"Unable to open settings file"<<endl;
//    return */;
//    QByteArray data;
//    data = settingFile.readAll();
//    QList<QByteArray> lst = data.split('=');
//    _serverPort = lst[1].toInt();
}
void UdpServer::readyRead()
{
    char buff[4],data[128];

    while(_udpSocket->bytesAvailable())
    {
        QHostAddress addr;
        quint16 port;
        _udpSocket->readDatagram(data,udpDataSize,&addr,&port);
        char ch = data[4];
        char buff[128];

        switch(ch)
        {
        case CMD::INFORM_AND_UPDATE:
        {
            CMD::FileInfo fileInfo(data+5);
            fileInfo.dump();
            QString key = QString::fromStdString(fileInfo.getFileName())
                    +":"+QString::fromStdString(fileInfo.getIp())
                    +":"+QString::number(fileInfo.getPort());
            if(!_clientList.contains(key))
            {
                _clientList.append(key);
                QPair<QString, int> pr;
                pr.first = QString::fromStdString(fileInfo.getIp());
                pr.second = fileInfo.getPort();
                _fileNameClientMap.insert(QString::fromStdString(fileInfo.getFileName()),pr);
            }
            RSP::AckFromServer ackFromServer(fileInfo.getSeq(),RSP::ACK_INFORM_AND_UPDATE_SERVER);
            ackFromServer.serialize(buff);
            _udpSocket->writeDatagram(buff,udpDataSize,addr,port);
            _udpSocket->waitForBytesWritten();
        }
            break;
        case CMD::QUERY_FOR_CONTENT:
        {
            char buff[128];
            int bytes;
            QString key;
            CMD::Query query(data+5);
            query.dump();
            QList<QPair<QString,int> > list =_fileNameClientMap.values(QString::fromStdString(query.getFileName()));
            if(!query.getQueryAll())
            {
                for(int i = 0; i < list.count(); i++)
                {
                    _seqNo++;
                    key = QString::number(_seqNo) +":"+addr.toString();
                    RSP::QueryOutput queryOutput;
                    queryOutput.setSeq(_seqNo);
                    queryOutput.setFileName(query.getFileName());
                    queryOutput.setIpAddr(list[i].first.toStdString());
                    queryOutput.setPort(list[i].second);
                    queryOutput.dump();
                    bytes = queryOutput.serialize(buff);
                    bytes=udpDataSize;
                    _clientData[key] = queryOutput;
                    _udpSocket->writeDatagram(buff,bytes,addr,port);
                    _udpSocket->waitForBytesWritten();
                    qDebug()<<"addr.toString(): "<<addr.toString();
                    RttTimer *timer = new RttTimer(_seqNo,QString::fromStdString(query.getIpAddr()),query.getPort(),_udpSocket);
                    _timerAckMap[key] = timer;
                    timer->timerStart();
                }
            }
        }
            break;
        case CMD::ACK_CONTENT_FROM_SERVER:
        {
            RSP::AckFromServer ackFromServer(data+5);
            ackFromServer.dump();
            QString key = QString::number(ackFromServer.getSeq()) +":"+
                    addr.toString();
            if(!_ackClienMap.contains(key))
            {
                _ackClienMap.append(key);
                if(_timerAckMap.contains(key))
                {
                    _timerAckMap[key]->stopTimer();
                    qDebug()<<"timer for ACK "+ QString::number(ackFromServer.getSeq()) +" stopped for Ip " + addr.toString();
                }
            }
        }
            break;
        case CMD::EXIT_BY_CLIENT:
        {
            CMD::ClientExit clientExit(data+5);
            clientExit.dump();
            QString key;
            QPair<QString,int> pr;
            pr.first = QString::fromStdString(clientExit.getIp());
            pr.second = clientExit.getPort();
            QList<QString> fileNames = _fileNameClientMap.keys(pr);
            for(int i =0; i<fileNames.count(); i++)
            {
                _fileNameClientMap.remove(fileNames[i],pr);
                key = fileNames[i] +":"+pr.first+":"+QString::number(pr.second);
                _clientList.removeAll(key);
            }
        }
        }

    }
}

