#include "thread.h"
#include "mainwindow.h"
#include <fstream>
#include <QFile>
QMap<QString,QString> uploaderMap;
void Thread::run()
{
    _socket = new QTcpSocket();
    _socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    if(!_socket->setSocketDescriptor(this->_socketDescriptor))
    {
        // something's wrong, we just emit a signal
        //emit error(_socket->error());
        return;
    }

    connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    // We'll have multiple clients, we want to know which is which
    qDebug() << this->_socketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies
    exec();
}
void Thread::readyRead()
{
    char buff[4],data[128];
    _socket->read(buff, 4);
    SIGNED_INT size ;
    memcpy(&size,buff,4);
    _socket->read(data,size);
    char ch = data[0];
    switch(ch)
    {
    case CMD::CLIENT_REQUEST:
    {
        CMD::ClientRequest clientRequest(data+1);
        if(uploaderMap.contains(QString::fromStdString(clientRequest.getFileName())))
        {
            STRING  fileName = clientRequest.getFileName();
            QString filePath = uploaderMap[QString::fromStdString(fileName)];
            int seq=0,bytes;
            char buf1[128];
            QFile inputFile(filePath);
            inputFile.open(QIODevice::ReadOnly);
            QByteArray read;
            int headerSize = QString::fromStdString(fileName).count() + 4 + 4 + 1 + 4 + 4;//addition 4,4,1 for other header values
            int dataSize = 128 - headerSize; //we need to send packet of max 128 bytes
            int numOfPackets = inputFile.size()%dataSize==0?inputFile.size()/dataSize:inputFile.size()/dataSize+1;
            CMD::ClientConfirmation clientConfirmation;
            clientConfirmation.setFileName(fileName);
            clientConfirmation.setTotalNoOfPackets(numOfPackets);
            bytes = clientConfirmation.serialize(buff);
            qDebug()<<"Client confirmation,bytes written:"<<_socket->write(buff,bytes);
            _socket->waitForBytesWritten();

            while(1)
            {
                read.clear();
                read = inputFile.read(dataSize);
                qDebug() << "Read : " << read;
                if(read.size()==0)
                    break;
                CMD::FileData d(seq,fileName,read.toStdString());
                bytes = d.serialize(buf1);

                qDebug()<<"file data,bytes written:"<<_socket->write(buf1,bytes);
                _socket->waitForBytesWritten();
                usleep(10000);
                seq++;
                read.clear();
            }

            inputFile.close();
            _socket->close();
        } else {
            char buff[128];
            CMD::ClientFileNotFound clientFileNotFound(clientRequest.getFileName());
            int bytes = clientFileNotFound.serialize(buff);
            qDebug()<<"ClientFileNotFound,bytes written:"<<_socket->write(buff,bytes);
            _socket->waitForBytesWritten();

            qDebug() << "File not found.Ending the thread for Client "+ QString::fromStdString(clientRequest.getUserName());
            _socket->disconnect();
        }
    }
        break;
    default:
        qDebug()<< "UKNOWN COMMAND";

    }


}

void Thread::disconnected()
{
    qDebug() << _socketDescriptor << " Disconnected";
    _socket->deleteLater();
    exit(0);
}
