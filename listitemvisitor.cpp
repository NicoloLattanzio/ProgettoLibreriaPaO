#include "listitemvisitor.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"

void ListItemVisitor::visit(const Library::Book& book) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(book.getTitle()),
             QString::fromStdString(book.getAuthor()),
             QString::number(book.getYear()));

    QSize iconSize(56, 56); // Aumenta le dimensioni a 32x32 pixel
    m_listItem = new QListWidgetItem(QIcon(":/icons/Resources/images/IconBook.png"), displayText);
    m_listItem->setData(Qt::DecorationRole, QIcon(":/icons/Resources/images/IconBook.png").pixmap(iconSize));
}

void ListItemVisitor::visit(const Library::Movie& movie) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(movie.getTitle()),
             QString::fromStdString(movie.getAuthor()),
             QString::number(movie.getYear()));

    QSize iconSize(56, 56); // Aumenta le dimensioni a 32x32 pixel
    m_listItem = new QListWidgetItem(QIcon(":/icons/Resources/images/IconMovie.png"), displayText);
    m_listItem->setData(Qt::DecorationRole, QIcon(":/icons/Resources/images/IconMovie.png").pixmap(iconSize));
}

void ListItemVisitor::visit(const Library::CD& cd) {
    QString displayText = QString("%1\n%2 - %3")
        .arg(QString::fromStdString(cd.getTitle()),
             QString::fromStdString(cd.getAuthor()),
             QString::number(cd.getYear()));

    QSize iconSize(56, 56); // Aumenta le dimensioni a 32x32 pixel
    m_listItem = new QListWidgetItem(QIcon(":/icons/Resources/images/IconCD.png"), displayText);
    m_listItem->setData(Qt::DecorationRole, QIcon(":/icons/Resources/images/IconCD.png").pixmap(iconSize));
}
