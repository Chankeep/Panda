#ifndef PANDASERVERTHREAD_H
#define PANDASERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <PandaSocket.h>

class PandaServer;

class PandaServerThread : public QThread
{
    Q_OBJECT
public:
    PandaServerThread(QObject* parent = nullptr);
    ~PandaServerThread() override = default;
    quint16 CurrentSocketNum;

    void CreateSocket(qintptr socketDescriptor);
protected:
    void run() override;

signals:
    void SignAddInfo(QString, int);

public:
    PandaServer* server;
    QList<PandaSocket*> socketList;

};

#endif // PANDASERVERTHREAD_H
