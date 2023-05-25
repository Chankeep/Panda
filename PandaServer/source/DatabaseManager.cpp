#include "DatabaseManager.h"

#include <QDebug>

bool DatabaseManager::Init()
{
    QSqlDatabase Db;
    Db = QSqlDatabase::addDatabase("QMYSQL");
    Db.setPort(21475);
    Db.setHostName("frp-any.top");
    Db.setDatabaseName("PandaDatabase");
    Db.setUserName("root");
    Db.setPassword("ckp20021209");
    if(!Db.open())
    {
        qDebug() << "Error: Failed to connect database. db:" << Db.lastError();
    }
    else
    {
        qDebug() << "Success to connect database";
        DbQuery = new QSqlQuery(Db);
    }
    return true;
}

bool DatabaseManager::Find(const QString &table, const QString &name, const QString &conditions)
{
    QString execStr = QString("select \"%1\" from \"%2\" \"%3\" ").arg(name).arg(table).arg(conditions);
    if(!DbQuery->exec(execStr))
    {
        return false;
    }
    return true;
}
