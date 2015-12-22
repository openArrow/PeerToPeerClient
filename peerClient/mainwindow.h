#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <iostream>
#include <QStandardItemModel>
#include<QUdpSocket>
#include "rtttimer.h"
#include "serverinfo.h"
#include "QTcpSocket"
#include "tcpserver.h"
namespace Ui {
class MainWindow;
}
extern QMap<QString,QString> uploaderMap;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void createRowUploader(QString fileName);
    void createRowDownloader(QString fileName, QString ip, int port);
    static QMap<int, CMD::FileInfo> getDataToserverMap() { return _dataToserverMap; }
    static QList<int> getAckSeqList() { return _ackSeqList; }
    //calculating time for timeout
    static int getTimerValue() {
        int val;
        _estimatedRtt    = 0.875 * _estimatedRtt + 0.125*_sampleRtt;
        _devRtt = (1 - 0.25) * _devRtt+  0.25* abs(_sampleRtt - _estimatedRtt );
        val = _estimatedRtt + 4*_devRtt;

        if(val<500)
        {
            val = 500;
            _estimatedRtt = 100;
            _sampleRtt = 200;
            _devRtt = 300;
        }
    qDebug()<<"timeOut Interval"<<val;
    return _estimatedRtt + 4*_devRtt;
}
static void setSampleRtt(int sampleRtt)
{
    _sampleRtt = sampleRtt;
}
static MainWindow* getMainWindowPtr() {return _mainWindowPtr;}
~MainWindow();

private slots:
void on_actionConnect_to_server_triggered();

void on_pushButtonBrowse_clicked();

void on_pushButtonDownload_clicked();
void updateDownloaderModel(QString filename, QString ip, int port,bool down);

void on_pushButtonQuery_clicked();
void readyRead();

void on_pushButtonUpload_clicked();

void on_actionStart_Server_triggered();
void updateDownloaderModelSlot(QString filename);

void on_actionExit_triggered();

private:

static QMap<int, CMD::FileInfo> _dataToserverMap;
static QList<int> _ackSeqList;
QMap<int,RttTimer*> _timerAckMap;
Ui::MainWindow *ui;
ServerInfo *_serverInfo;
QStandardItemModel _modelUploader;
QStandardItemModel _modelDownloader;
TcpServer *_tcpServer;
QUdpSocket *_udpSocket;
int _seqFileInfo;
static MainWindow *_mainWindowPtr;
static int _estimatedRtt;
static int _sampleRtt;
static int _devRtt;

};

#endif // MAINWINDOW_H
