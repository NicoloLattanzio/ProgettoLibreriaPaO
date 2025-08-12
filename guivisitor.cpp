#include "guivisitor.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"
#include <QPixmap>
#include <QLabel>


// Implementazione DisplayVisitor
void GUIVisitor::visit(const Library::Book& book) {
    QLabel* imageLabel = new QLabel;
    if (!QString::fromStdString(book.getImage()).isEmpty()) {
          QPixmap image(QString::fromStdString(book.getImage()));

          if (!image.isNull()) {
            imageLabel->setPixmap(image.scaled(250, 400, Qt::KeepAspectRatio));
            imageLabel->setAlignment(Qt::AlignLeft);
          } else {
            imageLabel->setText("Image not found");
          }
        }
        contentLayout->addWidget(imageLabel);


    result = QString(
        "<b>Title:</b> %1<br>"
        "<b>Author:</b> %2<br>"
        "<b>Year:</b> %3<br>"
        "<b>Genre:</b> %4<br>"
        "<b>Type:</b> Book<br>"
        "<b>Publisher:</b> %5<br>"
        "<b>Translator:</b> %6<br>"
        "<b>Pages:</b> %7")
        .arg(QString::fromStdString(book.getTitle()),
            QString::fromStdString(book.getAuthor()),
            QString::number(book.getYear()),
            QString::fromStdString(book.getGenre()),
            QString::fromStdString(book.getPublisher()),
            QString::fromStdString(book.getTranslator()),
            QString::number(book.getPages()));


}

void GUIVisitor::visit(const Library::Movie& movie) {
    QLabel* imageLabel = new QLabel;
    QPixmap image(QString::fromStdString(movie.getImage()));
    imageLabel->setPixmap(image.scaled(250, 400, Qt::KeepAspectRatio));
    result = QString(
        "<b>Title:</b> %1<br>"
        "<b>Author:</b> %2<br>"
        "<b>Year:</b> %3<br>"
        "<b>Genre:</b> %4<br>"
        "<b>Type:</b> Movie<br>"
        "<b>Language:</b> %5<br>"
        "<b>Min Age:</b> %6<br>"
        "<b>Duration:</b> %7 min<br>"
        "<b>Oscar:</b> %8")
        .arg(QString::fromStdString(movie.getTitle()),
            QString::fromStdString(movie.getAuthor()),
            QString::number(movie.getYear()),
            QString::fromStdString(movie.getGenre()),
            QString::fromStdString(movie.getLanguage()),
            QString::number(movie.getMinAge()),
            QString::number(movie.getDuration()),
            movie.getOscar() ? "Yes" : "No");
}

void GUIVisitor::visit(const Library::CD& cd) {
    QLabel* imageLabel = new QLabel;
    QPixmap image(QString::fromStdString(cd.getImage()));
    imageLabel->setPixmap(image.scaled(250, 400, Qt::KeepAspectRatio));
    result = QString(
        "<b>Title:</b> %1<br>"
        "<b>Author:</b> %2<br>"
        "<b>Year:</b> %3<br>"
        "<b>Genre:</b> %4<br>"
        "<b>Type:</b> CD<br>"
        "<b>Album:</b> %5<br>"
        "<b>Language:</b> %6<br>"
        "<b>Duration:</b> %7 min")
        .arg(QString::fromStdString(cd.getTitle()),
            QString::fromStdString(cd.getAuthor()),
            QString::number(cd.getYear()),
            QString::fromStdString(cd.getGenre()),
            QString::fromStdString(cd.getAlbum()),
            QString::fromStdString(cd.getLanguage()),
            QString::number(cd.getDuration()));
}
