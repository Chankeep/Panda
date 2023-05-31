#include "LoginManager.h"
#include "PandaServer.h"

#include <QMessageBox>

LoginManager::LoginManager(QObject* parent)
    : DbManager(DatabaseManager::GetManager()),
      server(static_cast<PandaServer*>(parent))
{
}

loginReturnType LoginManager::Login(const QString &account, const QString &password)
{
    if(!DbManager.Find("userinformation", "username, password", QString("account = \"%1\"").arg(account)))
        return loginReturnType::acNotExist;

    QString truePassword = DbQuery->value(1).toString();
    if(password != truePassword)
        return loginReturnType::wrongPw;

    return loginReturnType::noneLoginError;
}

signUpReturnType LoginManager::SignUp(const QString& userName, const QString &account, const QString &password)
{
    if(DbManager.Find("userinformation", "password", QString("account = \"%1\"").arg(account)))
        return signUpReturnType::alreayExisted;

    if(!DbManager.Add("userinformation", QString("(\"%1\", \"%2\", \"%3\")").arg(userName, account, password)))
        return signUpReturnType::unknown;

    return noneSignUpError;
}

bool LoginManager::ConnectDatabase(const QString& ip, uint port)
{
    if(!DbManager.Init(ip, port))
    {
        QMessageBox::warning(server->GetMainWindow(), "Error", "Database connection failure!");
        return false;
    }
    else
    {
        DbQuery = DbManager.GetQuery();
        return true;
    }
}

QString LoginManager::GetUserName()
{
    QString userName = DbQuery->value(0).toString();
    return userName;
}
