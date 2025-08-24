#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include "../model/Item.h"

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool initialize(const QString& path = "library.db");
    bool saveItem(Library::Item& item);
    //bool ModifyItem(Library::Item& item);
    bool DeleteItem(Library::Item& item);
    QVector<Library::Item*> loadAllItems();

private:
    QSqlDatabase m_db;
    DatabaseManager() = default;
};

#endif // DATABASEMANAGER_H
