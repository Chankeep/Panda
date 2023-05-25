#include "PandaServerThread.h"

#include "PandaServer.h"

PandaServerThread::PandaServerThread(QObject* parent) : QThread(parent), CurrentSocketNum(0)
{
    server = static_cast<PandaServer*>(parent);

}

void PandaServerThread::CreateSocket(qintptr socketDescriptor)
{
    PandaSocket* newSocket = new PandaSocket(socketDescriptor);

    connect(server, &PandaServer::SignSendMsg, newSocket, &PandaSocket::SlotWriteData);
    connect(server, &PandaServer::SignDisconnected, newSocket, &PandaSocket::SlotDisconnected);

    newSocket->setSocketDescriptor(socketDescriptor);
    socketList.append(newSocket);

    emit SignAddInfo("SB", socketDescriptor);
}

void PandaServerThread::run()
{
    connect(this,&PandaServerThread::SignAddInfo , server, &PandaServer::SlotAddSocketInfo);
    this->exec();
}

