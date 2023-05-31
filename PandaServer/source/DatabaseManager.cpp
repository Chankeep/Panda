#include "DatabaseManager.h"

#include <QDebug>

bool DatabaseManager::Init(const QString& ip, uint port)
{
    qDebug()<<ip;
    QSqlDatabase Db;
    Db = QSqlDatabase::addDatabase("QMYSQL");
    Db.setPort(port);
    Db.setHostName(ip);
    Db.setDatabaseName("PandaDatabase");
    Db.setUserName("root");
    Db.setPassword("ckp20021209");
    if(!Db.open())
    {
        qDebug() << "Error: Failed to connect database. db:" << Db.lastError();
        return false;
    }
    else
    {
        qDebug() << "Success to connect database";
        DbQuery = new QSqlQuery(Db);
    }
    return true;
}

bool DatabaseManager::Add(const QString &table, const QString &values)
{
    QString execStr = QString("INSERT INTO %1 (username, account, password) values %2").arg(table).arg(values);
    if(!exec(execStr))
        return false;
    return true;
}

bool DatabaseManager::Find(const QString &table, const QString &name, const QString &conditions)
{
    QString execStr = QString("select %1 from %2 where %3 ").arg(name).arg(table).arg(conditions);
    if(!DbQuery->exec(execStr))
    {
        return false;
    }
    DbQuery->first();
    if(DbQuery->value(0).isNull())
        return false;
    return true;
}

bool DatabaseManager::Delete(const QString &table, const QString &conditions)
{
    QString execStr = QString("DELETE FROM %1 WHERE %2").arg(table).arg(conditions);
    if(!exec(execStr))
        return false;
    return true;
}

bool DatabaseManager::Update(const QString &table, const QString &changes, const QString &conditions)
{
    QString execStr = QString("UPDATE %1 SET %2 WHERE %3").arg(table).arg(changes).arg(conditions);
    if(!exec(execStr))
        return false;
    return true;
}
