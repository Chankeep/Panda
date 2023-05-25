#include "PandaSocket.h"


PandaSocket::PandaSocket(qintptr socketDescriptor, QObject *parent) : QTcpSocket(parent), CurrentDescriptor(socketDescriptor)
{
    connect(this, &QTcpSocket::readyRead, this, &PandaSocket::SlotReadData);
    connect(this, &PandaSocket::disconnected, this, &PandaSocket::SlotDisconnected);
}

void PandaSocket::SlotReadData()
{
    QString msg = this->readAll();
    emit SignSendMsg(msg);
}

void PandaSocket::SlotDisconnected()
{
    qDebug() << this->socketDescriptor() << "disconnected";
    emit SignDisconnected(this->socketDescriptor());
}
