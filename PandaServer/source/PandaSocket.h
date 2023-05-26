#ifndef PANDASOCKET_H
#define PANDASOCKET_H

#include <QObject>
#include <QTcpSocket>

class PandaSocket : public QObject
{
    Q_OBJECT
public:
    PandaSocket(qintptr socketDescriptor);
    qintptr CurrentDescriptor;

    QTcpSocket* socket;

signals:
    void SignSocketDisconnected(qintptr);
    void SignSendClientMsg(QString);
    void SignSetDesc(qintptr);

public slots:
    void SlotReadClientData();
    void SlotSocketDisconnected();
    void SlotWriteData(QString);
    void SlotSetDesc(qintptr);

private:
};

#endif // PANDASOCKET_H
