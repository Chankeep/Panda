#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QHostAddress>

class DatabaseManager
{
public:
    static DatabaseManager& GetManager()
    {
        static DatabaseManager instance;
        return instance;
    }

    bool Init(const QString&, uint);
    bool Add(const QString& table, const QString& values);
    bool Find(const QString& table, const QString& name = "*", const QString& conditions = "");
    bool Delete(const QString& table, const QString& conditions = "");
    bool Update(const QString& table, const QString &changes, const QString& conditions = "");
    bool exec(const QString& query) { return DbQuery->exec(query); };

    QSqlQuery* GetQuery() { return DbQuery; }
private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager(const DatabaseManager&&) = delete;

    QSqlQuery* DbQuery;
};

#endif // DATABASEMANAGER_H
