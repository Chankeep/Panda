#include "PandaServerThread.h"

#include "PandaServer.h"

PandaServerThread::PandaServerThread(QObject* parent) : QThread(parent), CurrentSocketNum(0)
{
    server = static_cast<PandaServer*>(parent);
}

void PandaServerThread::run()
{
    qDebug() << "thread:" << currentThreadId() << " start!";

    connect(server, &PandaServer::SignSockethasDisconnected, this, &PandaServerThread::SlotSocketHasDisconnected);
    this->exec();
}

void PandaServerThread::SlotSocketHasDisconnected(qintptr descriptor)
{
    for(int i=0; i<socketList.count(); i++)
    {
        auto item = socketList.at(i);
        if(item->socket->socketDescriptor() == descriptor)
        {
            socketList.removeAt(i);
            return;
        }
    }
}


