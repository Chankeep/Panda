#ifndef PANDASERVER_H
#define PANDASERVER_H

#include <QMainWindow>

#include <QTcpServer>
#include <QCryptographicHash>

#include "PandaServerThread.h"
#include "LoginManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PandaServer; }
QT_END_NAMESPACE

struct SocketInformation{
    PandaSocket* socket;
    qintptr socketDescriptor;
    QString ip;
    QString port;
    QString userName;

    SocketInformation() = default;
    SocketInformation(PandaSocket* socket, qintptr desc, const QString& msg)
        : socket(socket), socketDescriptor(desc)
    {
        static int index = 0;
        userName = "Null" + QString::number(index++);
        auto addrAndPort = msg.split(":");
        ip = addrAndPort[0];
        port = addrAndPort[1];
    }

    void SetUserName(const QString& userName)
    {
        this->userName = userName;
    }
};

class LoginManager;

class PandaServer : public QTcpServer
{
    Q_OBJECT

public:
    PandaServer(QWidget *parent = nullptr);
    virtual ~PandaServer() override;
    void show() { mainWindow->show(); }
    void Login(const QString& account, const QString& password);
    void SignUp(const QString& account, const QString& password);

    void CreateThread();
    void DeleteThread();
    int GetMinLoadThreadIndex();

    void FlushSocketInfo();
    QStringList GetAllSocketInfo();

    QMainWindow* GetMainWindow() { return mainWindow; }
protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void SignAddInfo(PandaSocket*, QString, int);
    //send data to all PandaSocket
    void SignSendAllMsg(QString);
//    void SignSocketHasDisconnected(qintptr);
    void SignCreateSocket(qintptr);
    void SignSetDesc(qintptr);

    void SignForceDisconnect(qintptr);

public slots:

    void SlotConnectDatabase();
    void SlotBeginListen();

    void SlotCreateThread();
    void SlotDeleteThread();

    void SlotChangeSocketInfo(QString);
    void SlotAddSocketInfo(PandaSocket*, QString, qintptr);
    void SlotSendServerMsg();
    void SlotSendClinetMsg(QString);
    void SlotServerDisconnected(qintptr, QThread*);
    void SlotForceDisconnect();

    void SlotLogin(qintptr, QString, QString);
    void SlotSignUp(qintptr, QString, QString, QString);

private:
    QMainWindow* mainWindow;
    Ui::PandaServer *ui;

    QMap<qintptr, SocketInformation> socketInformations;
    QList<PandaServerThread*> threadList;
    QList<PandaSocket*> socketList;
    LoginManager* loginManager;

    quint16 CurrentSocketNum = 0;
};
#endif // PANDASERVER_H
