#include "DatabaseManager.h"
#include "DatabaseSaveVisitor.h"
#include "model/Item.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <stdexcept>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qDebug() << "Database Error on open:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query;
    bool success = true;

    // Enable foreign keys and create tables
    success &= query.exec("PRAGMA foreign_keys = ON;");
    if (!success) {
        qDebug() << "Error enabling foreign keys:" << query.lastError().text();
        return false;
    }

    success &= query.exec(
        "CREATE TABLE IF NOT EXISTS Items ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   title TEXT NOT NULL,"
        "   author TEXT,"
        "   genre TEXT,"
        "   year INTEGER,"
        "   image TEXT"
        ");"
    );
    if (!success) {
        qDebug() << "Error creating Items table:" << query.lastError().text();
        return false;
    }

    success &= query.exec(
        "CREATE TABLE IF NOT EXISTS Books ("
        "   item_id INTEGER PRIMARY KEY,"
        "   publisher TEXT,"
        "   translator TEXT,"
        "   pages INTEGER,"
        "   FOREIGN KEY(item_id) REFERENCES Items(id) ON DELETE CASCADE"
        ");"
    );
    if (!success) {
        qDebug() << "Error creating Books table:" << query.lastError().text();
        return false;
    }

    success &= query.exec(
        "CREATE TABLE IF NOT EXISTS Movies ("
        "   item_id INTEGER PRIMARY KEY,"
        "   language TEXT,"
        "   min_age INTEGER,"
        "   duration INTEGER,"
        "   oscar BOOLEAN,"
        "   trailer TEXT,"
        "   FOREIGN KEY(item_id) REFERENCES Items(id) ON DELETE CASCADE"
        ");"
    );
    if (!success) {
        qDebug() << "Error creating Movies table:" << query.lastError().text();
        return false;
    }

    success &= query.exec(
        "CREATE TABLE IF NOT EXISTS CDs ("
        "   item_id INTEGER PRIMARY KEY,"
        "   language TEXT,"
        "   album TEXT,"
        "   duration INTEGER,"
        "   audio_track TEXT,"
        "   FOREIGN KEY(item_id) REFERENCES Items(id) ON DELETE CASCADE"
        ");"
    );
    if (!success) {
        qDebug() << "Error creating CDs table:" << query.lastError().text();
        return false;
    }

    return success;
}

bool DatabaseManager::saveItem(Library::Item& item) {
    QSqlDatabase::database().transaction();
    QSqlQuery query;

    try {
        const bool isNew = (item.getItemID() == 0);
        qDebug() << "SaveItem called for item (isNew:" << isNew << ", current ID:" << item.getItemID() << ")";

        // Save/update common data
        if(isNew) {
            query.prepare(
                "INSERT INTO Items (title, author, genre, year, image) "
                "VALUES (:title, :author, :genre, :year, :image)"
            );
            qDebug() << "Preparing INSERT into Items table.";
        } else {
            query.prepare(
                "UPDATE Items SET "
                "title = :title, author = :author, genre = :genre, "
                "year = :year, image = :image "
                "WHERE id = :id"
            );
            query.bindValue(":id", item.getItemID());
            qDebug() << "Preparing UPDATE for Items table (ID:" << item.getItemID() << ").";
        }

        // Binding common values
        query.bindValue(":title", QString::fromStdString(item.getTitle()));
        query.bindValue(":author", QString::fromStdString(item.getAuthor()));
        query.bindValue(":genre", QString::fromStdString(item.getGenre()));
        query.bindValue(":year", item.getYear());
        query.bindValue(":image", QString::fromStdString(item.getImage()));

        if(!query.exec()) {
            qDebug() << "ERROR: Failed to execute Items table query. Last error:" << query.lastError().text();
            qDebug() << "Query executed:" << query.lastQuery();
            throw query.lastError();
        }

        // Handle ID for new objects
        int currentId = item.getItemID();
        if(isNew) {
            currentId = query.lastInsertId().toInt();
            item.setItemID(currentId);
            qDebug() << "New item saved, assigned ID:" << currentId;
        } else {
            qDebug() << "Item updated with ID:" << currentId;
        }

        // Save specific data with Visitor
        qDebug() << "Calling DatabaseSaveVisitor for item ID:" << currentId;
        DatabaseSaveVisitor visitor(currentId);
        item.accept(visitor);
        qDebug() << "DatabaseSaveVisitor finished for item ID:" << currentId;

        QSqlDatabase::database().commit();
        qDebug() << "Transaction committed successfully for item ID:" << item.getItemID();
        return true;

    } catch(const QSqlError& e) {
            QSqlDatabase::database().rollback();
            qDebug() << "Database Error (SQL Exception):" << e.text();
            qDebug() << "Last query attempted:" << query.lastQuery();
            return false;
        } catch(const std::exception& e) {
            QSqlDatabase::database().rollback();
            qDebug() << "Error (Standard Exception):" << e.what();
            return false;
        }
}


bool DatabaseManager::DeleteItem( Library::Item& item) {
    QSqlDatabase::database().transaction();
    QSqlQuery query;

    try {

        query.prepare("DELETE FROM Items WHERE id = :id");
        query.bindValue(":id", item.getItemID());

        qDebug() << "Preparing to delete item with ID:" << item.getItemID();

        if(!query.exec()) {
            qDebug() << "ERROR: Failed to delete item. Last error:" << query.lastError().text();
            qDebug() << "Query executed:" << query.lastQuery();
            throw query.lastError();
        }

        QSqlDatabase::database().commit();
        qDebug() << "Item deleted successfully with ID:" << item.getItemID();
        return true;

    } catch(const QSqlError& e) {
        QSqlDatabase::database().rollback();
        qDebug() << "Database Error (SQL Exception):" << e.text();
        qDebug() << "Last query attempted:" << query.lastQuery();
        return false;
    } catch(const std::exception& e) {
        QSqlDatabase::database().rollback();
        qDebug() << "Error (Standard Exception):" << e.what();
        return false;
    }
}


QVector<Library::Item*> DatabaseManager::loadAllItems() {
    QVector<Library::Item*> items;
    QSqlQuery query("SELECT * FROM Items");

    if(!query.exec()) {
        qDebug() << "ERROR loading items:" << query.lastError().text();
        return items;
    }

    while(query.next()) {
        int id = query.value("id").toInt();
        std::string title = query.value("title").toString().toStdString();
        std::string author = query.value("author").toString().toStdString();
        std::string genre = query.value("genre").toString().toStdString();
        unsigned int year = query.value("year").toUInt();
        std::string image = query.value("image").toString().toStdString();

        // Controlla il tipo specifico
        QSqlQuery typeQuery;
        typeQuery.prepare("SELECT * FROM Books WHERE item_id = :id");
        typeQuery.bindValue(":id", id);
        if(typeQuery.exec() && typeQuery.next()) {
            // È un libro
            std::string publisher = typeQuery.value("publisher").toString().toStdString();
            std::string translator = typeQuery.value("translator").toString().toStdString();
            unsigned int pages = typeQuery.value("pages").toUInt();

            items.append(new Library::Book(
                author, title, id, genre, year, image,
                publisher, translator, pages
            ));
            continue;
        }

        typeQuery.prepare("SELECT * FROM Movies WHERE item_id = :id");
        typeQuery.bindValue(":id", id);
        if(typeQuery.exec() && typeQuery.next()) {
            // È un film
            std::string language = typeQuery.value("language").toString().toStdString();
            unsigned int minAge = typeQuery.value("min_age").toUInt();
            unsigned int duration = typeQuery.value("duration").toUInt();
            bool oscar = typeQuery.value("oscar").toBool();
            std::string trailer = typeQuery.value("trailer").toString().toStdString();

            items.append(new Library::Movie(
                author, title, id, genre, year, image,
                language, minAge, duration, oscar, trailer
            ));
            continue;
        }

        typeQuery.prepare("SELECT * FROM CDs WHERE item_id = :id");
        typeQuery.bindValue(":id", id);
        if(typeQuery.exec() && typeQuery.next()) {
            // È un CD
            std::string language = typeQuery.value("language").toString().toStdString();
            std::string album = typeQuery.value("album").toString().toStdString();
            unsigned int duration = typeQuery.value("duration").toUInt();
            std::string audioTrack = typeQuery.value("audio_track").toString().toStdString();

            items.append(new Library::CD(
                author, title, id, genre, year, image,
                language, album, duration, audioTrack
            ));
            continue;
        }
    }
    return items;
}
