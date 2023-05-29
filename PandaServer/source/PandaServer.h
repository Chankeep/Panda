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
    QString userName;

    SocketInformation() = default;
    SocketInformation(PandaSocket* socket, qintptr desc, const QString& name)
        : socket(socket), socketDescriptor(desc), userName(name){}

    QString GetInfo()
    {
        return userName;
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

    void InitThread();
    int GetMinLoadThreadIndex();

    void FlushSocketComboBox();
    QStringList GetAllSocketInfo();

    QMainWindow* GetMainWindow() { return mainWindow; }
protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void SignAddInfo(PandaSocket*, QString, int);
    //send data to all PandaSocket
    void SignSendAllMsg(QString);
    void SignSockethasDisconnected(qintptr);
    void SignCreateSocket(qintptr);
    void SignSetDesc(qintptr);


public slots:
    void SlotAddSocketInfo(PandaSocket*, QString, qintptr);
    void SlotSendServerMsg();
    void SlotSendClinetMsg(QString);
    void SlotServerDisconnected(qintptr);

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
