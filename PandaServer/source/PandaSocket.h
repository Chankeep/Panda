#ifndef PANDASOCKET_H
#define PANDASOCKET_H

#include <QObject>
#include <QTcpSocket>

class PandaSocket : public QTcpSocket
{
    Q_OBJECT
public:
    PandaSocket(qintptr socketDescriptor, QObject* parent = nullptr);
signals:
    void SignDisconnected(int);
    void SignSendMsg(QString);
    void SignReceiveData(QString, int);

protected slots:
    void SlotReadData();
    void SlotDisconnected();
private:
    qintptr CurrentDescriptor;
};

#endif // PANDASOCKET_H
