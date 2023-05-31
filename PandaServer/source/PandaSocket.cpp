#include "PandaSocket.h"
#include <QThread>
#include <QHostAddress>
PandaSocket::PandaSocket(qintptr socketDescriptor) : CurrentDescriptor(socketDescriptor)
{
    qRegisterMetaType<qintptr>("qintptr");
}

PandaSocket::~PandaSocket()
{
    qDebug() << "PandaSocket who controled " << this->CurrentDescriptor << " has been delete";
}

void PandaSocket::SlotReadClientData()
{
    qDebug() << "read in thread: " << QThread::currentThreadId();
    QString msg = socket->readAll();

    if(msg[0] == '0')
    {
        auto loginInfo = msg.split(" ");
        emit SignLogin(socket->socketDescriptor(), loginInfo[1], loginInfo[2]);
    }
    else if(msg[0] == '1')
    {
        auto signUpInfo = msg.split(" ");
        emit SignalSignUp(socket->socketDescriptor(), signUpInfo[1], signUpInfo[2], signUpInfo[3]);
    }
    else if(msg[0] == 's')
    {
        emit SignSendClientMsg(msg);
    }
    else
        return;

}

void PandaSocket::SlotSocketDisconnected()
{
    socket->abort();
    socket->close();
    emit SignSocketDisconnected(this->CurrentDescriptor, QThread::currentThread());
    qDebug() << this->CurrentDescriptor << "disconnected";
    delete socket;
    this->deleteLater();
}

void PandaSocket::SlotWriteData(QString msg)
{
    qDebug() << "write in thread: " << QThread::currentThreadId();
    socket->write(msg.toUtf8());
}

void PandaSocket::SlotSetDesc(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << "init in " << QThread::currentThreadId();
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &PandaSocket::SlotReadClientData);
    connect(socket, &QTcpSocket::disconnected, this, &PandaSocket::SlotSocketDisconnected);
    connect(this, &PandaSocket::SignForceDisconnect, this, &PandaSocket::SlotForceDisconnect, Qt::ConnectionType::AutoConnection);

    connect(this, &PandaSocket::SignLoginReturn, this, &PandaSocket::SlotReturnInfo);
    connect(this, &PandaSocket::SignalSignUpReturn, this, &PandaSocket::SlotReturnInfo);

    socket->setSocketDescriptor(socketDescriptor);
    emit SignAddInfo(this, socket->peerAddress().toString() + ":" + QString::number(socket->peerPort()), socketDescriptor);
}

void PandaSocket::SlotReturnInfo(QString msg)
{
    qDebug() << "Return: " + msg;
    socket->write(msg.toUtf8());
}

void PandaSocket::SlotForceDisconnect()
{
    socket->write("-");
    this->socket->disconnectFromHost();
    this->socket->waitForDisconnected();
}
