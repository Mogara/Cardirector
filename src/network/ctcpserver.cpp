#include "ctcpserver.h"
#include "ctcpsocket.h"

CTcpServer::CTcpServer(QObject *parent)
    : QTcpServer(parent)
{
}

void CTcpServer::incomingConnection(qintptr handle)
{
    CTcpSocket *socket = new CTcpSocket(this);
    socket->setSocketDescriptor(handle);
    emit newSocket(socket);
}
