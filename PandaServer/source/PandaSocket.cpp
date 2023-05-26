#include "PandaSocket.h"
#include <QThread>

PandaSocket::PandaSocket(qintptr socketDescriptor) : CurrentDescriptor(socketDescriptor)
{
    qRegisterMetaType<qintptr>("qintptr");
}

void PandaSocket::SlotReadClientData()
{
    qDebug() << "read in thread: " << QThread::currentThreadId();
    QString msg = socket->readAll();
    emit SignSendClientMsg(msg);
}

void PandaSocket::SlotSocketDisconnected()
{
    qDebug() << this->CurrentDescriptor << "disconnected";
    emit SignSocketDisconnected(socket->socketDescriptor());
    socket->abort();
    delete socket;
}

void PandaSocket::SlotWriteData(QString msg)
{
    qDebug() << "write in thread: " << QThread::currentThreadId();
    socket->write(msg.toLatin1());
}

void PandaSocket::SlotSetDesc(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << "init in " << QThread::currentThreadId();
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &PandaSocket::SlotReadClientData);
    connect(socket, &QTcpSocket::disconnected, this, &PandaSocket::SlotSocketDisconnected);
    socket->setSocketDescriptor(socketDescriptor);
}
