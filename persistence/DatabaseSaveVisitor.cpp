#include "DatabaseSaveVisitor.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <stdexcept>

DatabaseSaveVisitor::DatabaseSaveVisitor(int itemId) : m_itemId(itemId) {}

void DatabaseSaveVisitor::visit(const Library::Book& book) {
    QSqlQuery query;
    query.prepare(
        "INSERT OR REPLACE INTO Books (item_id, publisher, translator, pages) "
        "VALUES (:id, :publisher, :translator, :pages)"
    );
    query.bindValue(":id", m_itemId);
    query.bindValue(":publisher", QString::fromStdString(book.getPublisher()));
    query.bindValue(":translator", QString::fromStdString(book.getTranslator()));
    query.bindValue(":pages", book.getPages());

    qDebug() << "Visitor: Saving Book data for item ID:" << m_itemId;
    if (!query.exec()) {
        qDebug() << "Visitor ERROR: Failed to execute Book insert/replace for item ID" << m_itemId << ":" << query.lastError().text();
        throw query.lastError();
    }
    qDebug() << "Visitor: Successfully saved Book data for item ID:" << m_itemId;
}

void DatabaseSaveVisitor::visit(const Library::Movie& movie) {
    QSqlQuery query;
    query.prepare(
        "INSERT OR REPLACE INTO Movies (item_id, language, min_age, duration, oscar) "
        "VALUES (:id, :language, :min_age, :duration, :oscar)"
    );
    query.bindValue(":id", m_itemId);
    query.bindValue(":language", QString::fromStdString(movie.getLanguage()));
    query.bindValue(":min_age", movie.getMinAge());
    query.bindValue(":duration", movie.getDuration());
    query.bindValue(":oscar", movie.getOscar());

    qDebug() << "Visitor: Saving Movie data for item ID:" << m_itemId;
    if (!query.exec()) {
        qDebug() << "Visitor ERROR: Failed to execute Movie insert/replace for item ID" << m_itemId << ":" << query.lastError().text();
        throw query.lastError();
    }
    qDebug() << "Visitor: Successfully saved Movie data for item ID:" << m_itemId;
}

void DatabaseSaveVisitor::visit(const Library::CD& cd) {
    QSqlQuery query;
    query.prepare(
        "INSERT OR REPLACE INTO CDs (item_id, language, album, duration) "
        "VALUES (:id, :language, :album, :duration)"
    );
    query.bindValue(":id", m_itemId);
    query.bindValue(":language", QString::fromStdString(cd.getLanguage()));
    query.bindValue(":album", QString::fromStdString(cd.getAlbum()));
    query.bindValue(":duration", cd.getDuration());

    qDebug() << "Visitor: Saving CD data for item ID:" << m_itemId;
    if (!query.exec()) {
        qDebug() << "Visitor ERROR: Failed to execute CD insert/replace for item ID" << m_itemId << ":" << query.lastError().text();
        throw query.lastError();
    }
    qDebug() << "Visitor: Successfully saved CD data for item ID:" << m_itemId;
}
