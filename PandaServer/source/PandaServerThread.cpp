#include "PandaServerThread.h"

#include "PandaServer.h"

PandaServerThread::PandaServerThread(QObject* parent) : QThread(parent), CurrentSocketNum(0)
{
    server = static_cast<PandaServer*>(parent);
}


void PandaServerThread::run()
{
    qDebug() << "thread:" << currentThreadId() << " start!";

    this->exec();
}

