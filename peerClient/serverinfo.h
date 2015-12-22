#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QDialog>
#include <qmessagebox.h>
namespace Ui {
class ServerInfo;
}

class ServerInfo : public QDialog
{
    Q_OBJECT

public:
    static ServerInfo* getInstance()
    {
        if(_serverInfo== 0)
            _serverInfo = new ServerInfo();
        return _serverInfo;
    }
    QString getServerIp() { return _serverIp; }
    int getServerPort() { return _serverPort; }
    QString getUserName() { return _userName;}
    QString getMyIp() { return _myIp; }
    int getMyPort() { return _myPort; }

    ~ServerInfo();

private slots:
    void on_pushButtonOk_pressed();

private:
    Ui::ServerInfo *ui;
    QString _userName;
    QString _serverIp;
    int _serverPort;
    QString _myIp;
    int _myPort;

    static ServerInfo *_serverInfo;
    explicit ServerInfo(QWidget *parent = 0);
};

#endif // SERVERINFO_H
