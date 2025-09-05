#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QVector>

namespace Library {
    class Item;
}

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool initialize(const QString& path = "library.db");
    bool saveItem(Library::Item& item);
    bool deleteItem(Library::Item& item);
    QVector<Library::Item*> loadAllItems();
    void closeDatabase();

    QSqlDatabase database() const { return m_db; }

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    static const QString CONNECTION_NAME;
};

#endif // DATABASEMANAGER_H
