#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QTcpSocket* socket, QWidget *parent = nullptr);
    ~Login();
    void ColorInit();
    void signInReceive(char);
    void SignUpReceive(char);
signals:
    void SignShowClient();
    void SignCloseLogin();

private:
    QTcpSocket *loginSocket;
    Ui::Login *ui;

private slots:
    void SlotTest();
    void SlotWantSignup();
    void SlotWantBack();
    void SlotSignin();
    void SlotSignUp();


};

#endif // LOGIN_H
