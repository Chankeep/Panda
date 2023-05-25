#ifndef PANDASERVER_H
#define PANDASERVER_H

#include <QMainWindow>

#include <QTcpServer>
#include <QCryptographicHash>

#include "PandaServerThread.h"
#include "DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PandaServer; }
QT_END_NAMESPACE

struct SocketInformation{
    qintptr socketDescriptor;
    QString userName;

    SocketInformation(qintptr desc, const QString& name) : socketDescriptor(desc), userName(name){}

    QString GetInformation()
    {
        QString msg = "UserName: "+ userName + "socket descriptor: " + QString::number(socketDescriptor);
        return msg;
    }
};

class PandaServer : public QTcpServer
{
    Q_OBJECT

public:
    PandaServer(QWidget *parent = nullptr);
    ~PandaServer();
    void show() { mainWindow->show(); }
    bool Check(const QString& account);
    void Login(const QString& account, const QString& password);
    void SignUp(const QString& account, const QString& password);

    int GetMinLoadThreadIndex();
    void InitThread();
    void CreateSocket(qintptr socketDescriptor);
protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void SignAddInfo(QString, int);
    void SignSendMsg(QString);
    void SignDisconnected(qintptr);

public slots:
    void SlotAddSocketInfo(QString, qintptr);
    void SlotSignUp();
    void SlotSendServerMsg();
    void SlotSendClinetMsg(QString);
    void SlotDisconnected(qintptr);

private:
    QMainWindow* mainWindow;
    Ui::PandaServer *ui;

    QList<SocketInformation> socketInformations;
    QList<PandaServerThread*> threadList;
    QList<PandaSocket*> socketList;
    DatabaseManager& dbManager;
};
#endif // PANDASERVER_H
