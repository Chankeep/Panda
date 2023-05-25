#ifndef PANDASERVER_H
#define PANDASERVER_H

#include <QMainWindow>

#include <QThread>
#include <QTcpServer>
#include <QCryptographicHash>

#include "PandaSocket.h"
#include "DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PandaServer; }
QT_END_NAMESPACE

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

protected:
    void incomingConnection(qintptr handle) override;

public slots:
    void SlotSignUp();
    void SlotSendServerMsg();
    void SlotSendClinetMsg(QString);
    void SlotDisconnected(int);

private:
    QMainWindow* mainWindow;
    Ui::PandaServer *ui;

    QList<PandaSocket*> PandaSocketList;
    DatabaseManager& DbManager;
};
#endif // PANDASERVER_H
