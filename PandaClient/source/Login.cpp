#include "Login.h"
#include "ui_Login.h"
#include <QDebug>
#include<QMainWindow>
#include <QCryptographicHash>
Login::Login(QTcpSocket* socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login),
    loginSocket(socket)
{
    ui->setupUi(this);

    ColorInit();
    connect(this, &Login::SignCloseLogin, this, &Login::close);


    connect(ui->PushButtonSignUp,&QPushButton::clicked,this,&Login::SlotWantSignup);
    connect(ui->PushButtonBackspace,&QPushButton::clicked,this,&Login::SlotWantBack);
//    connect(ui->testButton, &QPushButton::clicked, this, &Login::SlotTest);
    connect(ui->PushButtonSignIn,&QPushButton::clicked,this,&Login::SlotSignin);
    connect(ui->PushButtonSignUp2,&QPushButton::clicked,this,&Login::SlotSignUp);



}

Login::~Login()
{
    delete ui;
}

void Login::ColorInit()
{


    this->setStyleSheet("background-color: rgb(20,196,188);"
                        "border-radius:15px;");
    ui->PasswordIn->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->AccountIn->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                 "color: rgb(31,31,31);"		//字体颜色
                                 "padding-left:20px;"       //内边距-字体缩进
                                 "background-color: rgb(255, 255, 255);" //背景颜色
                                 "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置

    ui->PushButtonSignIn->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed
    ui->PushButtonSignUp->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed
    ui->PasswordUp->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->RePasswordUp->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                 "color: rgb(31,31,31);"		//字体颜色
                                 "padding-left:20px;"       //内边距-字体缩进
                                 "background-color: rgb(255, 255, 255);" //背景颜色
                                 "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->UserNameUP->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                 "color: rgb(31,31,31);"		//字体颜色
                                 "padding-left:20px;"       //内边距-字体缩进
                                 "background-color: rgb(255, 255, 255);" //背景颜色
                                 "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->Account->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                 "color: rgb(31,31,31);"		//字体颜色
                                 "padding-left:20px;"       //内边距-字体缩进
                                 "background-color: rgb(255, 255, 255);" //背景颜色
                                 "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->PushButtonSignUp2->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed
    ui->PushButtonBackspace->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed
}

void Login::SlotTest()
{
    emit this->SignCloseLogin();
    emit this->SignShowClient();
}

void Login::SlotWantSignup()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Login::SlotWantBack()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Login::SlotSignin()
{
    qDebug()<<"fasongxinhao";
    QString Account = ui->AccountIn->text();
    QString Password = ui->PasswordIn->text();
    QByteArray PasswordMD5 = QCryptographicHash::hash(Password.toUtf8(), QCryptographicHash::Md5).toHex();
    QString data = "0 "+Account+" "+PasswordMD5;
    qDebug() << data;

    loginSocket->write(data.toUtf8()); //qt5去除了.toAscii()


}

void Login::SlotSignUp()
{
    qDebug()<<"zhuchexinhao";
    QString UserName = ui->UserNameUP->text();
    QString Account = ui->Account->text();
    QString Password = ui->PasswordUp->text();
    QString RePassword = ui->RePasswordUp->text();

    if(UserName.size()>20)
        ui->LabInformation->setText("用户名超过20个字符");
    if(Account.size()>20)
        ui->LabInformation->setText("账号超过20个字符");
    if(Password.size()>30)
        ui->LabInformation->setText("密码超过30个字符");
    if(Password!=RePassword)
        ui->LabInformation->setText("两次输入密码不一致");
    QByteArray PasswordMD5 = QCryptographicHash::hash(Password.toUtf8(), QCryptographicHash::Md5).toHex();
    QString data = "1 "+UserName+ " " +Account+" "+PasswordMD5;
    if(UserName.size()<20 && Account.size()<20 && Password.size()<30 && Password==RePassword)
    {
        loginSocket->write(data.toUtf8());
    }


}

void Login::signInReceive(char receive)
{
    if(receive == '0')
    {
        emit this->SignCloseLogin();
        emit this->SignShowClient();
    }
    else if(receive == '1')
    {
        ui->label->setText("账户或密码不正确");
    }
    else if(receive == '2')
    {
        ui->label->setText("账户不存在");
    }

    else
    {
        ui->label->setText("连接失败");
    }

}

void Login::SignUpReceive(char receive)
{
//    //4账户已存在5未知错误3注册成功返回登录
    if(receive == '4')
    {
        ui->LabInformation->setText("账户已存在");

    }
    else if(receive == '5')
    {
        ui->LabInformation->setText("未知错误");

    }
    else if(receive == '6')
    {
        ui->LabInformation->setText("注册成功请返回登录");
    }

}
