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
    void SignDisconnected(qintptr);
    void SignSendMsg(QString);

public slots:
    void SlotReadData();
    void SlotDisconnected();
    void SlotWriteData(QString);

private:
    qintptr CurrentDescriptor;
};

#endif // PANDASOCKET_H
