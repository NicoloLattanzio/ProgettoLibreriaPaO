#include "guivisitor.h"

#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCoreApplication>
#include <QMediaPlayer>


GUIVisitor::GUIVisitor()
    : resultWidget(new QWidget()),
      mainLayout(new QVBoxLayout())
{
    resultWidget->setLayout(mainLayout);
}

QWidget* GUIVisitor::getResultWidget() const {
    return resultWidget;
}

void GUIVisitor::clear() {
    QLayoutItem* child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
}

void GUIVisitor::visit(const Library::Book& book) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();

    QString path = QCoreApplication::applicationDirPath() + "/images/" +
                   QString::fromStdString(book.getImage());
    QLabel* imageLabel = new QLabel;
    qDebug() << "Percorso immagine (dal DB):" << path;
    QPixmap image(path);
    if (!image.isNull()){
        imageLabel->setPixmap(image.scaled(250, 250, Qt::KeepAspectRatio));
    }
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);
    hLayout->setAlignment(imageLabel, Qt::AlignLeft | Qt::AlignTop);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(book.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(book.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(book.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(book.getGenre())));
    form->addRow("Publisher:", new QLabel(QString::fromStdString(book.getPublisher())));
    form->addRow("Translator:", new QLabel(QString::fromStdString(book.getTranslator())));
    form->addRow("Pages:", new QLabel(QString::number(book.getPages())));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    mainLayout->addLayout(hLayout);
}

void GUIVisitor::visit(const Library::Movie& movie) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();
    QString path = QCoreApplication::applicationDirPath() + "/images/" +
                   QString::fromStdString(movie.getImage());
    QLabel* imageLabel = new QLabel;
    QPixmap image(path);
    if (!image.isNull())
        imageLabel->setPixmap(image.scaled(150, 200, Qt::KeepAspectRatio));
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(movie.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(movie.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(movie.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(movie.getGenre())));
    form->addRow("Language:", new QLabel(QString::fromStdString(movie.getLanguage())));
    form->addRow("Min Age:", new QLabel(QString::number(movie.getMinAge())));
    form->addRow("Duration:", new QLabel(QString::number(movie.getDuration())));
    form->addRow("Oscar:", new QLabel(movie.getOscar() ? "Yes" : "No"));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    mainLayout->addLayout(hLayout);
}

void GUIVisitor::visit(const Library::CD& cd) {
    clear();

    QHBoxLayout* hLayout = new QHBoxLayout();
    QString path = QCoreApplication::applicationDirPath() + "/images/" +
                   QString::fromStdString(cd.getImage());
    QLabel* imageLabel = new QLabel;
    QPixmap image(path);
    if (!image.isNull())
        imageLabel->setPixmap(image.scaled(150, 200, Qt::KeepAspectRatio));
    else
        imageLabel->setText("Image not found");
    hLayout->addWidget(imageLabel);

    QFormLayout* form = new QFormLayout;
    form->addRow("Title:", new QLabel(QString::fromStdString(cd.getTitle())));
    form->addRow("Author:", new QLabel(QString::fromStdString(cd.getAuthor())));
    form->addRow("Year:", new QLabel(QString::number(cd.getYear())));
    form->addRow("Genre:", new QLabel(QString::fromStdString(cd.getGenre())));
    form->addRow("Album:", new QLabel(QString::fromStdString(cd.getAlbum())));
    form->addRow("Language:", new QLabel(QString::fromStdString(cd.getLanguage())));
    form->addRow("Duration:", new QLabel(QString::number(cd.getDuration())));

    QWidget* formWidget = new QWidget;
    formWidget->setLayout(form);
    hLayout->addWidget(formWidget);

    mainLayout->addLayout(hLayout);
}
