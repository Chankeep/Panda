#include "LoginManager.h"
#include "PandaServer.h"

#include <QMessageBox>

LoginManager::LoginManager(QObject* parent)
    : DbManager(DatabaseManager::GetManager()),
      server(static_cast<PandaServer*>(parent))
{
    if(!DbManager.Init())
    {
        QMessageBox::warning(server->GetMainWindow(), "Error", "Database connection failure!");
    }
    else
    {
        DbQuery = DbManager.GetQuery();
        //test
//        qDebug() << Login("2961404413", "123456");
        qDebug() << signUp("djl", "787736837", "123456");
    }
}

loginReturnType LoginManager::Login(const QString &account, const QString &password)
{
    if(!DbManager.Find("userinformation", "password", QString("account = \"%1\"").arg(account)))
        return loginReturnType::acNotExist;

    QString truePassword = DbQuery->value(0).toString();
    if(password != truePassword)
        return loginReturnType::wrongPw;

    return loginReturnType::noneLoginError;
}

signUpReturnType LoginManager::signUp(const QString& userName, const QString &account, const QString &password)
{
    if(DbManager.Find("userinformation", "password", QString("account = \"%1\"").arg(account)))
        return signUpReturnType::alreayExisted;

    if(!DbManager.Add("userinformation", QString("(\"%1\", \"%2\", \"%3\")").arg(userName, account, password)))
        return signUpReturnType::unknown;
    return noneSignUpError;
}
