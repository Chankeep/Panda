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

protected:
    void run() override;

public:
    PandaServer* server;
};

#endif // PANDASERVERTHREAD_H
