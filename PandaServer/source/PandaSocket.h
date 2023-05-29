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
    void SignAddInfo(PandaSocket*, QString, qintptr);

    void SignLoginReturn(QString);
    void SignalSignUpReturn(QString);

    void SignLogin(qintptr, QString, QString);
    void SignalSignUp(qintptr, QString, QString, QString);

public slots:
    void SlotReadClientData();
    void SlotSocketDisconnected();
    void SlotWriteData(QString);
    void SlotSetDesc(qintptr);

    void SlotReturnInfo(QString);

private:
};

#endif // PANDASOCKET_H
