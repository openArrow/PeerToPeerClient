#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commands.h"
#include "threaddownloader.h"
#include "rtttimer.h"
#include <QUdpSocket>
MainWindow* MainWindow::_mainWindowPtr = NULL;
QList<int> MainWindow::_ackSeqList;
QMap<int, CMD::FileInfo> MainWindow::_dataToserverMap;
int MainWindow::_estimatedRtt = 100;
int MainWindow::_sampleRtt = 200;
int MainWindow::_devRtt = 300;
int udpDataSize = 50;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _estimatedRtt = 100;
    _devRtt = 200;
    _sampleRtt = 100;
    _mainWindowPtr = this;
    _serverInfo= ServerInfo::getInstance();
    ui->setupUi(this);
    ui->tableViewDownloads->setModel(&_modelDownloader);
    ui->tableViewUploads->setModel(&_modelUploader);
    QStringList headerList;
    headerList<<"FILE NAME";
    _modelUploader.setHorizontalHeaderLabels(headerList);
    headerList.clear();
    headerList<<"FILE NAME"<<"STATUS"<<"IP"<<"PORT";
    _modelDownloader.setHorizontalHeaderLabels(headerList);
    ui->tableViewDownloads->adjustSize();
    ui->tableViewUploads->adjustSize();
    _tcpServer = new TcpServer();
    //_tcpServer->startServer();
    _udpSocket = new QUdpSocket();
    connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    _seqFileInfo = 0;
    //hiding some buttons
    ui->pushButtonQueryAll->hide();
    ui->pushButtonEndDownload->hide();
    ui->pushButtonEndUpload->hide();
    ui->tableViewDownloads->hideColumn(1);

    ui->textEditLog->setReadOnly(true);

}
void MainWindow::createRowUploader(QString fileName)
{
    cout<<"_modelUploader.rowCount():"<<_modelUploader.rowCount()<<endl;
    _modelUploader.insertRow(_modelUploader.rowCount());
    QStandardItem *first = new QStandardItem(fileName);
    first->setEditable(false);
    QStandardItem *checkBox = new QStandardItem();
    checkBox->setCheckable(true);
    _modelUploader.setItem(_modelUploader.rowCount()-1,0,first);
}
void MainWindow::createRowDownloader(QString fileName, QString ip, int port)
{
    cout<<"_modelDownloader.rowCount():"<<_modelDownloader.rowCount()<<endl;
    QString status = "";
    QStandardItem *filename = new QStandardItem(fileName);
    filename->setEditable(false);
    QStandardItem *stat = new QStandardItem();
    stat->setData(QVariant(status),Qt::DisplayRole);
    stat->setEditable(false);
    QStandardItem *Ip = new QStandardItem();
    Ip->setData(QVariant(ip),Qt::DisplayRole);
    Ip->setEditable(false);
    QStandardItem *portNo = new QStandardItem();
    portNo->setEditable(false);
    portNo->setData(QVariant(port),Qt::DisplayRole);
    QList<QStandardItem*> lst;
    lst<<filename<<stat<<Ip<<portNo;


    _modelDownloader.appendRow(lst);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateDownloaderModel(QString filename, QString ip, int port,bool down)
{
    for(int i=0; i<_modelDownloader.rowCount(); i++)
    {
        QModelIndex index;
        if(_modelDownloader.index(i,0).data().toString() == filename &&
                _modelDownloader.index(i,2).data().toString() == ip &&
                _modelDownloader.index(i,3).data().toInt() == port)
        {
            index = _modelDownloader.index(i,1);
            if(down)
                _modelDownloader.setData(index,"Downloading",Qt::DisplayRole);
            else
                _modelDownloader.setData(index,"Completed",Qt::DisplayRole);

        }
    }

}
void MainWindow::on_actionConnect_to_server_triggered()
{
    _serverInfo->exec();
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    QString fileNamePath = QFileDialog::getOpenFileName(this, tr("Open File"));
    if(fileNamePath.isEmpty())
        return;
    QStringList list = fileNamePath.split('/');
    QString fileName = list[list.count()-1];
    if(uploaderMap.contains(fileName))
    {
        QMessageBox::information(this,"ERROR","This File is already present in the table");
        return;
    }
    uploaderMap[fileName] = fileNamePath;
    createRowUploader(fileName);
}

void MainWindow::on_pushButtonDownload_clicked()
{
    int row = ui->tableViewDownloads->selectionModel()->currentIndex().row();
    _modelDownloader.setData(_modelDownloader.index(row,1),QVariant("DOWNLOADING"),Qt::DisplayRole);
    QString filename = _modelDownloader.index(row,0).data().toString();
    QString ip = _modelDownloader.index(row,2).data().toString();
    int port = _modelDownloader.index(row,3).data().toInt();
    ThreadDownloader *thread = new ThreadDownloader(filename.toStdString(),_serverInfo->getMyIp().toStdString(),_serverInfo->getMyPort(),
                                                    ip.toStdString(),port,this);
    thread->start();
}

void MainWindow::on_pushButtonQuery_clicked()
{
    if(ui->lineEditFile->text().isEmpty())
    {
        QMessageBox::information(this,"Missing","Please Enter File to Query");
        return;
    }
    CMD::Query query;
    query.setFileName(ui->lineEditFile->text().toStdString());
    query.setIpAddr(_serverInfo->getMyIp().toStdString());
    query.setPort(_serverInfo->getMyPort());
    query.setQueryAll(false);

    char buff[128];
    _udpSocket->bind(QHostAddress::AnyIPv4,7777);
    int bytes = query.serialize(buff);
    qDebug()<<"bytes written"<<_udpSocket->writeDatagram(buff,bytes,QHostAddress(_serverInfo->getServerIp()),_serverInfo->getServerPort());}
void MainWindow::readyRead()
{

    char data[128];
    QHostAddress addr;
    quint16 port;
    while(_udpSocket->bytesAvailable())
    {
        _udpSocket->readDatagram(data, udpDataSize, &addr, &port);
        char ch = data[4];
        switch(ch)
        {
        case RSP::QUERY_OUTPUT:
        {
            char buff[128];
            RSP::QueryOutput queryOutput(data+5);
            createRowDownloader(QString::fromStdString(queryOutput.getFileName()),
                                QString::fromStdString(queryOutput.getIpAddr()),queryOutput.getPort());
            RSP::AckFromServer ackFromServer(queryOutput.getSeq(),CMD::ACK_CONTENT_FROM_SERVER);
            ackFromServer.dump();
            ackFromServer.serialize(buff);
            qDebug()<<"Bytes written for Query Output Ack:"<<_udpSocket->writeDatagram(buff,udpDataSize,QHostAddress(_serverInfo->getServerIp()),
                                      _serverInfo->getServerPort());
            _udpSocket->waitForBytesWritten();
        }
            break;
        case RSP::ACK_INFORM_AND_UPDATE_SERVER:
        {
            RSP::AckFromServer ackFromServer(data+5);
            if(!_ackSeqList.contains(ackFromServer.getSeq()))
            {
                _ackSeqList.append(ackFromServer.getSeq());
                if(_timerAckMap.contains(ackFromServer.getSeq()))
                {
                 setSampleRtt(_timerAckMap[ackFromServer.getSeq()]->getTimeElapsed());
                _timerAckMap[ackFromServer.getSeq()]->stopTimer();
                }
                qDebug()<<"timer for ACK "+ QString::number(ackFromServer.getSeq()) +" stopped";

            }
        }
            break;
        default:
            qDebug()<<"UNKNOWN COMMAND";
        }
    }
}

void MainWindow::on_pushButtonUpload_clicked()
{

    int seq =0;
    for(int i = 0; i<_modelUploader.rowCount(); i++)
    {
        CMD::FileInfo fileInfo;
        _seqFileInfo++;
        fileInfo.setSeq(_seqFileInfo);
        fileInfo.setFileName(_modelUploader.index(i,0).data(Qt::DisplayRole).toString().toStdString());
        fileInfo.setIp(_serverInfo->getMyIp().toStdString());
        fileInfo.setPort(_serverInfo->getMyPort());
        _dataToserverMap[seq] = fileInfo;
        fileInfo.dump();
        char buff[128];
        _udpSocket->bind(QHostAddress::AnyIPv4,7777);
        int bytes = fileInfo.serialize(buff);
        bytes=udpDataSize;
        qDebug()<<"bytes written"<<_udpSocket->writeDatagram(buff,bytes,QHostAddress(_serverInfo->getServerIp()),_serverInfo->getServerPort());
        _udpSocket->waitForBytesWritten();
        RttTimer *timer = new RttTimer(_seqFileInfo,_udpSocket);
        _timerAckMap[_seqFileInfo] = timer;
        timer->timerStart();
    }
}

void MainWindow::on_actionStart_Server_triggered()
{
    _tcpServer->startServer(_serverInfo->getMyPort());
}
void MainWindow::updateDownloaderModelSlot(QString filename)
{
    QTextCursor prev_cursor = ui->textEditLog->textCursor();
    ui->textEditLog->moveCursor(QTextCursor::End);
    ui->textEditLog->insertPlainText(filename+" downloading completed \n");
}


void MainWindow::on_actionExit_triggered()
{
    char buff[128];
    CMD::ClientExit clientExit;
    clientExit.setIp(_serverInfo->getMyIp().toStdString());
    clientExit.setPort(_serverInfo->getMyPort());
    int bytes = clientExit.serialize(buff);
    bytes=udpDataSize;
    qDebug()<<"bytes written"<<_udpSocket->writeDatagram(buff,bytes,QHostAddress(_serverInfo->getServerIp()),_serverInfo->getServerPort());
    _udpSocket->waitForBytesWritten();
}
