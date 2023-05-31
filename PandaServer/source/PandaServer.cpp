#include "PandaServer.h"
#include "ui_PandaServer.h"

#include <QDebug>
#include <QStringListModel>

PandaServer::PandaServer(QWidget *parent)
    : QTcpServer(parent),
    ui(new Ui::PandaServer)
{
    mainWindow = new QMainWindow();
    ui->setupUi(this->mainWindow);
    ui->centralwidget->setFixedSize(mainWindow->size());

    loginManager = new LoginManager(this);

    connect(ui->comboBox, SIGNAL(activated(QString)), this, SLOT(SlotChangeSocketInfo(QString)));
    connect(ui->threadCreateButton, &QPushButton::clicked, this, &PandaServer::SlotCreateThread);
    connect(ui->threadDeleteButton, &QPushButton::clicked, this, &PandaServer::SlotDeleteThread);
    connect(ui->disconnectedButton, &QPushButton::clicked, this, &PandaServer::SlotForceDisconnect);

    connect(ui->dbConnectButton, &QPushButton::clicked, this, &PandaServer::SlotConnectDatabase);
    connect(ui->listenButton, &QPushButton::clicked, this, &PandaServer::SlotBeginListen);
    connect(ui->ClientSendButton, &QPushButton::clicked, this, &PandaServer::SlotSendServerMsg);
    connect(this, &PandaServer::SignAddInfo, this, &PandaServer::SlotAddSocketInfo);

    SlotCreateThread();
    SlotBeginListen();
}

PandaServer::~PandaServer()
{
    DeleteThread();
    delete ui;
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

void PandaServer::CreateThread()
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

void PandaServer::DeleteThread()
{
    for(auto thread : threadList)
    {
        qDebug() << thread << "has been deleted";
        thread->quit();
        thread->wait();
        thread->deleteLater();
    }
    threadList.clear();
}

void PandaServer::FlushSocketInfo()
{
    ui->comboBox->clear();
    QStringList list = GetAllSocketInfo();
    if(list.empty())
    {
        ui->disconnectedButton->setEnabled(false);
        return;
    }
    else
    {
        ui->comboBox->addItems(GetAllSocketInfo());
        ui->socketAddrLabel->setText(socketInformations.first().ip);
        ui->socketPortLabel->setText(socketInformations.first().port);
        ui->disconnectedButton->setEnabled(true);
    }
}

QStringList PandaServer::GetAllSocketInfo()
{
    QStringList socketInfos;
    for(auto info : socketInformations)
    {
        socketInfos.append(info.userName);
    }
    return socketInfos;
}

void PandaServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new connection. descriptor: " << socketDescriptor;

    int index = GetMinLoadThreadIndex();
    PandaSocket* newSocket = new PandaSocket(socketDescriptor);
    if(index < 0)//主线程
    {
        qDebug() << "Create socket in mainThread"  << "\nThere is " << CurrentSocketNum << " sockets in current thread!";
        this->CurrentSocketNum++;

        connect(newSocket, &PandaSocket::SignSocketDisconnected, this, &PandaServer::SlotServerDisconnected);
//        emit newSocket->SignSetDesc(socketDescriptor);
    }
    else//子线程
    {
        newSocket->moveToThread(threadList[index]);
        qDebug() << "Create socket in subThread"  << "\nThere is " << threadList[index]->CurrentSocketNum << " sockets in current thread!";
        threadList[index]->CurrentSocketNum++;

        connect(newSocket, &PandaSocket::SignSocketDisconnected, this, &PandaServer::SlotServerDisconnected);

    }
    connect(newSocket, &PandaSocket::SignLogin, this, &PandaServer::SlotLogin, Qt::ConnectionType::QueuedConnection);
    connect(newSocket, &PandaSocket::SignalSignUp, this, &PandaServer::SlotSignUp, Qt::ConnectionType::QueuedConnection);
    connect(this, &PandaServer::SignSendAllMsg, newSocket, &PandaSocket::SlotWriteData, Qt::ConnectionType::QueuedConnection);
    connect(newSocket, &PandaSocket::SignAddInfo, this, &PandaServer::SlotAddSocketInfo, Qt::ConnectionType::QueuedConnection);
    connect(newSocket, &PandaSocket::SignSendClientMsg, this, &PandaServer::SlotSendClinetMsg, Qt::ConnectionType::QueuedConnection);

    connect(newSocket, &PandaSocket::SignSetDesc, newSocket, &PandaSocket::SlotSetDesc);

    emit newSocket->SignSetDesc(socketDescriptor);
    socketList.append(newSocket);
}

void PandaServer::SlotForceDisconnect()
{
    QString currentName = ui->comboBox->currentText();
    for(auto& info : socketInformations)
    {
        if(info.userName == currentName)
        {
            emit socketInformations[info.socketDescriptor].socket->SignForceDisconnect();
            return;
        }
    }
}

void PandaServer::SlotConnectDatabase()
{
    QString ip = ui->dbAddrEdit->text();
    uint port = ui->dbPortEdit->text().toUInt();
    if(loginManager->ConnectDatabase(ip, port))
    {
        ui->dbConnectButton->setEnabled(false);
        ui->dbAddrEdit->setEnabled(false);
        ui->dbPortEdit->setEnabled(false);
    }
}

void PandaServer::SlotBeginListen()
{
    auto ip = ui->listenAddrEdit->text();
    auto port = ui->listenPortEdit->text().toUInt();
    if(this->listen(QHostAddress(ip), port))
    {
        qDebug() << "listen begin!";
        ui->listenButton->setEnabled(false);
        ui->listenAddrEdit->setEnabled(false);
        ui->listenPortEdit->setEnabled(false);
    }
}

void PandaServer::SlotCreateThread()
{
    CreateThread();
    ui->threadCreateButton->setEnabled(false);
    ui->threadDeleteButton->setEnabled(true);
}

void PandaServer::SlotDeleteThread()
{
    DeleteThread();
    ui->threadCreateButton->setEnabled(true);
    ui->threadDeleteButton->setEnabled(false);
}

void PandaServer::SlotChangeSocketInfo(QString userName)
{
    for(auto info : socketInformations)
    {
        if(info.userName == userName)
        {
            ui->socketAddrLabel->setText(info.ip);
            ui->socketPortLabel->setText(info.port);
            return;
        }
    }
}

void PandaServer::SlotAddSocketInfo(PandaSocket* socket, QString msg, qintptr socketDescriptor)
{
    SocketInformation info = {socket, socketDescriptor, msg};
    socketInformations[socketDescriptor] = info;
    FlushSocketInfo();
}


void PandaServer::SlotSendServerMsg()
{
    QString msg = ui->serverSend->toPlainText();
    if(msg == "")
        return;
    msg = "v " + msg;
    qDebug() << "send message!" << msg;

    emit SignSendAllMsg(msg);
}

void PandaServer::SlotSendClinetMsg(QString msg)
{
    emit SignSendAllMsg(msg);

    ui->socketSended_2->append(msg);
}

void PandaServer::SlotServerDisconnected(qintptr descriptor, QThread* thread)
{
    socketInformations.remove(descriptor);
    FlushSocketInfo();
    if(thread != QThread::currentThread())
    {
        for(auto& t : threadList)
        {
            if(t == thread)
            {
                t->CurrentSocketNum--;
                return;
            }
        }
    }
    CurrentSocketNum--;

}

void PandaServer::SlotLogin(qintptr socketDescriptor, QString account, QString password)
{
    auto result = loginManager->Login(account, password);
    if(result == loginReturnType::wrongPw)
    {
        emit socketInformations[socketDescriptor].socket->SignLoginReturn("1");
    }
    else if(result == loginReturnType::acNotExist)
    {
        emit socketInformations[socketDescriptor].socket->SignLoginReturn("2");
    }
    else
    {
        QString userName = loginManager->GetUserName();
        qDebug() << userName + " has been login!";
        emit socketInformations[socketDescriptor].socket->SignLoginReturn("0");
        socketInformations[socketDescriptor].SetUserName(userName);
        FlushSocketInfo();
    }

}

void PandaServer::SlotSignUp(qintptr socketDescriptor,QString userName, QString account, QString password)
{
    auto result = loginManager->SignUp(userName, account, password);
    if(result == signUpReturnType::alreayExisted)
    {
        emit socketInformations[socketDescriptor].socket->SignalSignUpReturn("4");
    }
    else if(result == signUpReturnType::unknown)
    {
        emit socketInformations[socketDescriptor].socket->SignalSignUpReturn("5");
    }
    else
        emit socketInformations[socketDescriptor].socket->SignalSignUpReturn("3");
}
