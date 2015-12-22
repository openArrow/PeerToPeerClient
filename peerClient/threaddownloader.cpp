#include "threaddownloader.h"
#include <QFile>
#include "mainwindow.h"

void ThreadDownloader::run()
{
    if(_socket->state() == QAbstractSocket::UnconnectedState)
    {
        disconnected();
        return;
    }
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this,SIGNAL(updateDownloaderModel(QString)),MainWindow::getMainWindowPtr(),SLOT(updateDownloaderModelSlot(QString)));

    exec();
}

void ThreadDownloader::readyRead()
{
    char buff[4],data[128];
    while(_socket->bytesAvailable())
    {
        _socket->read(buff, 4);
        SIGNED_INT size ;
        memcpy(&size,buff,4);
        _socket->read(data,size);
        char ch = data[0];
        switch(ch)
        {
        case CMD::FRAGEMENT_FILE_DATA:
        {
            CMD::FileData filedata(data+1);
            std::cout<<"data:"<<filedata.getData()<<endl;
            _dataMap[filedata.getSeqNo()] = QString::fromStdString(filedata.getData());
            //check if all packets are recieved or not
            for(int i = 0; i < _numOfpackets;i++)
            {
                if(!_dataMap.contains(i))
                                    break;
                else if(i == _numOfpackets-1)
                    writeToFile();
            }
        }
            break;
        case CMD::CLIENT_CONFIMATION_FOR_UPLOADING:
        {
            //information regarding file to be downloadaed sent by uploader
            CMD::ClientConfirmation clientConfirmation(data+1);
            _numOfpackets = clientConfirmation.getTotalNoOfPackets();
        }
            break;
        case CMD::CLIENT_FILE_NOT_FOUND:
        {
            CMD::ClientFileNotFound clientFileNotFound(data+1);
            qDebug()<<QString::fromStdString(clientFileNotFound.getFileName())+" file not found at Peer Client";
        }

       }
    }
}
void ThreadDownloader::writeToFile()
{
    QFile outFile(QString::fromStdString(_filename));
    outFile.open(QIODevice::WriteOnly);
    for(int i = 0; i < _dataMap.count();i++)
    {
        outFile.write(_dataMap[i].toLocal8Bit());
    }
    outFile.close();
    emit updateDownloaderModel(QString::fromStdString(_filename));

}
void ThreadDownloader::disconnected()
{
    qDebug() << " Disconnected Downloader Thread";
    _socket->deleteLater();
    exit(0);
}
