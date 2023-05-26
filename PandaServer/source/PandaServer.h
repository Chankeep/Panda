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
    SocketInformation() = default;
    SocketInformation(qintptr desc, const QString& name) : socketDescriptor(desc), userName(name){}

    QString GetInfo()
    {
        return "用户名： " + userName + "desc: " + socketDescriptor;
    }
};

class PandaServer : public QTcpServer
{
    Q_OBJECT

public:
    PandaServer(QWidget *parent = nullptr);
    virtual ~PandaServer() override;
    void show() { mainWindow->show(); }
    bool Check(const QString& account);
    void Login(const QString& account, const QString& password);
    void SignUp(const QString& account, const QString& password);

    int GetMinLoadThreadIndex();
    void InitThread();
    void FlushSocketComboBox();
    QStringList GetAllSocketInfo();
protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void SignAddInfo(QString, int);
    //send data to all PandaSocket
    void SignSendAllMsg(QString);
    void SignSockethasDisconnected(qintptr);
    void SignCreateSocket(qintptr);
    void SignSetDesc(qintptr);

public slots:
    void SlotAddSocketInfo(QString, qintptr);
    void SlotSendServerMsg();
    void SlotSendClinetMsg(QString);
    void SlotServerDisconnected(qintptr);

private:
    QMainWindow* mainWindow;
    Ui::PandaServer *ui;

    QMap<qintptr, SocketInformation> socketInformations;
    QList<PandaServerThread*> threadList;
    QList<PandaSocket*> socketList;
    DatabaseManager& dbManager;

    quint16 CurrentSocketNum = 0;
};
#endif // PANDASERVER_H
