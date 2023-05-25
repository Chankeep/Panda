#ifndef PANDACLIENT_H
#define PANDACLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class PandaClient; }
QT_END_NAMESPACE

class PandaClient : public QMainWindow
{
    Q_OBJECT

public:
    PandaClient(QWidget *parent = nullptr);
    ~PandaClient();

private:
    Ui::PandaClient *ui;
    QTcpSocket *TcpClient;

private slots:
    void SlotReadData();


    void SlotSend();
};
#endif // PANDACLIENT_H
