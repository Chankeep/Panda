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
    qDebug() << this->CurrentDescriptor << "disconnected";
    emit SignDisconnected(this->socketDescriptor());
}

void PandaSocket::SlotWriteData(QString msg)
{
    this->write(msg.toLatin1());
}
