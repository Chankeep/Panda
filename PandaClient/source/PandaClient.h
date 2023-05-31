#ifndef PANDACLIENT_H
#define PANDACLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include "Login.h"
QT_BEGIN_NAMESPACE
namespace Ui { class PandaClient; }
QT_END_NAMESPACE
class Login;
class PandaClient : public QMainWindow
{
    Q_OBJECT

public:
    PandaClient(QWidget *parent = nullptr);
    ~PandaClient();
    void ColorInit();
    void FontInit();
private:
    Ui::PandaClient *ui;
    QTcpSocket *TcpClient;
    Login *LoginIn;

private slots:
    void SlotReadData();
    void SlotSend();
    void SlotFontMode(QFont &font);


};
#endif // PANDACLIENT_H
