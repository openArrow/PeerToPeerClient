#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include "thread.h"
class TcpServer : QTcpServer
{
    Q_OBJECT
   public:
       explicit TcpServer(QObject *parent = 0);
       void startServer(int port);
   signals:

   public slots:

   protected:
       void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
