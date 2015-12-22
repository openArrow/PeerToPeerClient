#include <QCoreApplication>
#include "udpserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UdpServer *udpServer = new UdpServer();
    return a.exec();
}

