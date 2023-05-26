#include "PandaServer.h"
#include "ui_PandaServer.h"

#include <QDebug>
#include <QStringListModel>

PandaServer::PandaServer(QWidget *parent)
    : QTcpServer(parent),
    ui(new Ui::PandaServer),
    dbManager(DatabaseManager::GetManager())
{
    mainWindow = new QMainWindow();
    ui->setupUi(this->mainWindow);
    ui->centralwidget->setSizeIncrement(mainWindow->sizeHint());

    connect(ui->ClientSendButton, &QPushButton::clicked, this, &PandaServer::SlotSendServerMsg);
    connect(this, &PandaServer::SignAddInfo, this, &PandaServer::SlotAddSocketInfo);
    dbManager.Init();
    InitThread();

    this->listen(QHostAddress::Any, 8080);

    qDebug() << "listen begin!";
}

PandaServer::~PandaServer()
{
    for(auto thread : threadList)
    {
        thread->quit();
        thread->wait();
    }
    delete ui;
}

bool PandaServer::Check(const QString &account)
{
    if(dbManager.Find(account))
    {
        return true;
    }
    return false;
}

int PandaServer::GetMinLoadThreadIndex()
{
    int n = threadList.count();
    if(n == 0)
        return -1;
    quint16 minLoad = this->CurrentSocketNum;
    int index = -1;
    for(int i=0; i<n; i++)
    {
        if(threadList[i]->CurrentSocketNum < minLoad)
        {
            index = i;
            minLoad = threadList[i]->CurrentSocketNum;
        }
    }
    return index;
}

void PandaServer::InitThread()
{
    auto threadNum = ui->threadNumEdit->text().toInt();
    for(int i=0; i<threadNum; i++)
    {
        PandaServerThread* newThread = new PandaServerThread(this);
        threadList.append(newThread);
        newThread->start();
        qDebug() << "Create thread:" << newThread;
    }
}

void PandaServer::FlushSocketComboBox()
{
    ui->comboBox->clear();
    ui->comboBox->addItems(GetAllSocketInfo());
}

QStringList PandaServer::GetAllSocketInfo()
{
    QStringList socketInfos;
    for(auto info : socketInformations)
    {
        socketInfos.append(info.GetInfo());
    }
    return socketInfos;
}

void PandaServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new connection. descriptor: " << socketDescriptor;

    int index = GetMinLoadThreadIndex();
    if(index < 0)//主线程
    {
        PandaSocket* newSocket = new PandaSocket(socketDescriptor);
        qDebug() << "Create socket in mainThread"  << "\nThere is " << CurrentSocketNum << " sockets in current thread!";
        this->CurrentSocketNum++;

        connect(this, &PandaServer::SignSetDesc, newSocket, &PandaSocket::SlotSetDesc);
        connect(this, &PandaServer::SignSendAllMsg, newSocket, &PandaSocket::SlotWriteData);
        connect(this, &PandaServer::SignSockethasDisconnected, newSocket, &PandaSocket::SlotSocketDisconnected);
        connect(newSocket, &PandaSocket::SignSendClientMsg, this, &PandaServer::SlotSendClinetMsg);
        connect(newSocket, &PandaSocket::SignSocketDisconnected, this, &PandaServer::SlotServerDisconnected);

        emit this->SignSetDesc(socketDescriptor);
        emit SignAddInfo("Main", socketDescriptor);
        socketList.append(newSocket);
    }
    else//子线程
    {
        PandaSocket* newSocket = new PandaSocket(socketDescriptor);
        newSocket->moveToThread(threadList[index]);
        qDebug() << "Create socket in subThread"  << "\nThere is " << threadList[index]->CurrentSocketNum << " sockets in current thread!";
        threadList[index]->CurrentSocketNum++;

        connect(this, &PandaServer::SignSendAllMsg, newSocket, &PandaSocket::SlotWriteData);
        connect(this, &PandaServer::SignSockethasDisconnected, newSocket, &PandaSocket::SlotSocketDisconnected);
        connect(newSocket, &PandaSocket::SignSendClientMsg, this, &PandaServer::SlotSendClinetMsg);
        connect(newSocket, &PandaSocket::SignSetDesc, newSocket, &PandaSocket::SlotSetDesc);

        emit newSocket->SignSetDesc(socketDescriptor);
        emit SignAddInfo("Sub", socketDescriptor);
        threadList[index]->socketList.append(newSocket);
    }

//    PandaSocket* socket = new PandaSocket(socketDescriptor);

//    connect(socket, &PandaSocket::SignSendMsg, this, &PandaServer::SlotSendClinetMsg);
//    connect(socket, &PandaSocket::SignDisconnected, this, &PandaServer::SlotDisconnected);

//    socket->setSocketDescriptor(socketDescriptor);
//    PandaSocketList.append(socket);

}

void PandaServer::SlotAddSocketInfo(QString userName, qintptr socketDescriptor)
{
    static int index = 0;
    SocketInformation info = {socketDescriptor, userName};
    socketInformations[index++] = info;

    FlushSocketComboBox();
}


void PandaServer::SlotSendServerMsg()
{
    QString msg = ui->serverSend->toPlainText();
    if(msg == "")
        return;

    qDebug() << "send message!";

    emit SignSendAllMsg(msg);
//    for(int i=0; i<socketList.count(); i++)
//    {
//        auto item = socketList.at(i);
//        if(item->socket->write(msg.toLatin1()))
//        {
//            qDebug() << "send to socket:" << item->socket->socketDescriptor();
//            continue;
//        }
//    }
}

void PandaServer::SlotSendClinetMsg(QString msg)
{
//    emit updateServer(msg, length);
    emit SignSendAllMsg(msg);

    ui->socketSended_2->append(msg);
//    for(int i=0; i<socketList.count(); i++)
//    {
//        auto item = socketList.at(i);
//        if(item->socket->write(msg.toLatin1()))
//        {
//            continue;
//        }
//    }
}

void PandaServer::SlotServerDisconnected(qintptr descriptor)
{
    socketInformations.remove(descriptor);
    FlushSocketComboBox();
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
