#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QObject>
#include "DatabaseManager.h"
class PandaServer;

enum loginReturnType{
    acNotExist,
    wrongPw,
    noneLoginError
};

enum signUpReturnType{
    alreayExisted,
    unknown,
    noneSignUpError
};

class LoginManager : public QObject
{
    Q_OBJECT
public:
    LoginManager(QObject* parent = nullptr);

    loginReturnType Login(const QString& account, const QString& password);
    signUpReturnType signUp(const QString& userName, const QString& account, const QString& password);
private:
    DatabaseManager& DbManager;
    PandaServer* server;

    QSqlQuery* DbQuery;
};

#endif // LOGINMANAGER_H
