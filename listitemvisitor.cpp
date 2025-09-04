#include "listitemvisitor.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"

void ListItemVisitor::visit(const Library::Book& book) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(book.getTitle()),
             QString::fromStdString(book.getAuthor()),
             QString::number(book.getYear()));

    m_listItem = new QListWidgetItem(QIcon(":/icons/images/IconBook.png"), displayText);
}

void ListItemVisitor::visit(const Library::Movie& movie) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(movie.getTitle()),
             QString::fromStdString(movie.getAuthor()),
             QString::number(movie.getYear()));

    m_listItem = new QListWidgetItem(QIcon(":/icons/images/IconMovie.png"), displayText);
}

void ListItemVisitor::visit(const Library::CD& cd) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(cd.getTitle()),
             QString::fromStdString(cd.getAuthor()),
             QString::number(cd.getYear()));

    m_listItem = new QListWidgetItem(QIcon(":/icons/images/IconCD.png"), displayText);
}
