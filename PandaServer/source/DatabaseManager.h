#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& GetManager()
    {
        static DatabaseManager instance;
        return instance;
    }

    bool Init();
    void Add(const QString& table, const QString& values);
    bool Find(const QString& table, const QString& name = "*", const QString& conditions = "");
    void Delete(const QString& table, const QString& conditions = "");
    void Update(const QString& table, const QString& before, const QString& after, const QString& conditions = "");

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager(const DatabaseManager&&) = delete;

    QSqlQuery* DbQuery;
};

#endif // DATABASEMANAGER_H
