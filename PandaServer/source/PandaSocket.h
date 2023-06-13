#ifndef PANDASOCKET_H
#define PANDASOCKET_H

#include <QObject>
#include <QTcpSocket>

class PandaSocket : public QObject
{
    Q_OBJECT
public:
    PandaSocket(qintptr socketDescriptor);
    ~PandaSocket();
    qintptr CurrentDescriptor;

    QTcpSocket* socket;

signals:
    void SignForceDisconnect();
    void SignSocketDisconnected(qintptr, QThread*);
    void SignSetDesc(qintptr);
    void SignAddInfo(PandaSocket*, QString, qintptr);

    void SignLoginReturn(QString);
    void SignalSignUpReturn(QString);

    void SignLogin(qintptr, QString, QString);
    void SignalSignUp(qintptr, QString, QString, QString);

    void SignSendPublicMsg(QString);

public slots:
    void SlotReadClientData();
    void SlotSocketDisconnected();
    void SlotWriteData(QString);
    void SlotSetDesc(qintptr);

    void SlotReturnInfo(QString);
    void SlotForceDisconnect();
private:
};

#endif // PANDASOCKET_H
