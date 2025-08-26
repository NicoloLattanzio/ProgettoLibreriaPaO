#include "edititemwidget.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDate>
#include <QMessageBox>

EditItemWidget::EditItemWidget(QWidget *parent) :
    QWidget(parent),
    m_currentItem(nullptr),
    m_currentItemId(0)
{
    setWindowTitle("Modifica Item");
    setMinimumWidth(400);

    setupUI();

    // Creazione Prototipi per il Visitor
    m_prototypes.append(new Library::Book());
    m_prototypes.append(new Library::Movie());
    m_prototypes.append(new Library::CD());

    // Impostazione dello stato iniziale della UI
    onTypeChanged(0);
}

EditItemWidget::~EditItemWidget() {
    qDeleteAll(m_prototypes);
}

void EditItemWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Combo box per il tipo
    m_typeComboBox = new QComboBox(this);
    m_typeComboBox->addItem("Libro");
    m_typeComboBox->addItem("Film");
    m_typeComboBox->addItem("CD");
    mainLayout->addWidget(m_typeComboBox);

    // Connessione per il cambio di tipo
    connect(m_typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EditItemWidget::onTypeChanged);

    // Layout per i campi comuni
    QFormLayout *commonFormLayout = new QFormLayout();
    m_titleEdit = new QLineEdit(this);
    m_authorEdit = new QLineEdit(this);
    m_genreEdit = new QLineEdit(this);
    m_yearEdit = new QSpinBox(this);
    m_imageEdit = new QLineEdit(this);

    m_yearEdit->setRange(1800, QDate::currentDate().year());
    m_imageEdit->setPlaceholderText("URL o percorso file (opzionale)");

    commonFormLayout->addRow("Titolo:", m_titleEdit);
    commonFormLayout->addRow("Autore/Regista:", m_authorEdit);
    commonFormLayout->addRow("Genere:", m_genreEdit);
    commonFormLayout->addRow("Anno:", m_yearEdit);
    commonFormLayout->addRow("Immagine:", m_imageEdit);

    mainLayout->addLayout(commonFormLayout);

    // Creazione dei campi specifici
    createSpecificFields();
    mainLayout->addStretch();

    // Pulsanti
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("Salva Modifiche", this);
    m_cancelButton = new QPushButton("Annulla", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Connessioni dei pulsanti
    connect(m_saveButton, &QPushButton::clicked, this, &EditItemWidget::onSaveButtonClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &EditItemWidget::onCancelButtonClicked);
}

void EditItemWidget::createSpecificFields() {
    QVBoxLayout* mainLayout = static_cast<QVBoxLayout*>(layout());

    // Campi specifici per libro
    m_bookFields = new QWidget(this);
    QFormLayout* bookLayout = new QFormLayout(m_bookFields);
    m_publisherEdit = new QLineEdit(m_bookFields);
    m_translatorEdit = new QLineEdit(m_bookFields);
    m_pagesEdit = new QSpinBox(m_bookFields);
    m_pagesEdit->setRange(1, 10000);
    bookLayout->addRow("Editore:", m_publisherEdit);
    bookLayout->addRow("Traduttore:", m_translatorEdit);
    bookLayout->addRow("Pagine:", m_pagesEdit);
    mainLayout->addWidget(m_bookFields);

    // Campi specifici per film
    m_movieFields = new QWidget(this);
    QFormLayout* movieLayout = new QFormLayout(m_movieFields);
    m_movieLanguageEdit = new QLineEdit(m_movieFields);
    m_minAgeEdit = new QSpinBox(m_movieFields);
    m_durationMovieEdit = new QSpinBox(m_movieFields);
    m_oscarCheckBox = new QCheckBox(m_movieFields);
    m_movieTrailerEdit = new QLineEdit(m_movieFields);
    m_durationMovieEdit->setRange(1, 600);
    movieLayout->addRow("Lingua:", m_movieLanguageEdit);
    movieLayout->addRow("Età Minima:", m_minAgeEdit);
    movieLayout->addRow("Durata (min):", m_durationMovieEdit);
    movieLayout->addRow("Vincitore Oscar:", m_oscarCheckBox);

    movieLayout->addRow("Trailer:", m_movieTrailerEdit);
    mainLayout->addWidget(m_movieFields);

    m_cdFields = new QWidget(this);
    QFormLayout* cdLayout = new QFormLayout(m_cdFields);
    m_cdLanguageEdit = new QLineEdit(m_cdFields);
    m_albumEdit = new QLineEdit(m_cdFields);
    m_durationCDEdit = new QSpinBox(m_cdFields);
    m_audioTrackEdit = new QLineEdit(m_cdFields);
    m_durationCDEdit->setRange(1, 300);
    cdLayout->addRow("Lingua:", m_cdLanguageEdit);
    cdLayout->addRow("Album:", m_albumEdit);
    cdLayout->addRow("Durata (min):", m_durationCDEdit);
    cdLayout->addRow("Traccia Audio:", m_audioTrackEdit);
    mainLayout->addWidget(m_cdFields);
}

void EditItemWidget::setItem(const Library::Item* item) {
    if (!item) return;

    m_currentItem = item;
    m_currentItemId = item->getItemID();

    // Popola i campi comuni
    populateCommonFields();

    // Popola i campi specifici usando il visitor pattern
    item->accept(*this);
}

void EditItemWidget::populateCommonFields() {
    if (!m_currentItem) return;

    m_titleEdit->setText(QString::fromStdString(m_currentItem->getTitle()));
    m_authorEdit->setText(QString::fromStdString(m_currentItem->getAuthor()));
    m_genreEdit->setText(QString::fromStdString(m_currentItem->getGenre()));
    m_yearEdit->setValue(m_currentItem->getYear());
    m_imageEdit->setText(QString::fromStdString(m_currentItem->getImage()));
}

Library::Item* EditItemWidget::getUpdatedItem() const {
    if (!m_currentItem) {
        QMessageBox::warning(const_cast<EditItemWidget*>(this), "Errore", "Nessun elemento da modificare.");
        return nullptr;
    }

    if (m_titleEdit->text().isEmpty()) {
        QMessageBox::warning(const_cast<EditItemWidget*>(this), "Dati Mancanti", "Il campo Titolo è obbligatorio.");
        return nullptr;
    }

    int typeIndex = m_typeComboBox->currentIndex();
    std::string title = m_titleEdit->text().toStdString();
    std::string author = m_authorEdit->text().toStdString();
    std::string genre = m_genreEdit->text().toStdString();
    unsigned int year = m_yearEdit->value();
    std::string image = m_imageEdit->text().toStdString();

    // Usa l'ID esistente dell'item
    int id = m_currentItemId;

    if (typeIndex == 0) { // Libro
        return new Library::Book(author, title, id, genre, year, image,
                                 m_publisherEdit->text().toStdString(),
                                 m_translatorEdit->text().toStdString(),
                                 m_pagesEdit->value());
    } else if (typeIndex == 1) { // Film
        return new Library::Movie(author, title, id, genre, year, image,
                                  m_movieLanguageEdit->text().toStdString(),
                                  m_minAgeEdit->value(),
                                  m_durationMovieEdit->value(),
                                  m_oscarCheckBox->isChecked(),
                                  m_movieTrailerEdit->text().toStdString());

    } else if (typeIndex == 2) { // CD
        return new Library::CD(author, title, id, genre, year, image,
                               m_cdLanguageEdit->text().toStdString(),
                               m_albumEdit->text().toStdString(),
                               m_durationCDEdit->value(),
                               m_audioTrackEdit->text().toStdString());
    }
    return nullptr;
}

void EditItemWidget::visit(const Library::Book& book) {
    m_typeComboBox->setCurrentIndex(0);

    m_publisherEdit->setText(QString::fromStdString(book.getPublisher()));
    m_translatorEdit->setText(QString::fromStdString(book.getTranslator()));
    m_pagesEdit->setValue(book.getPages());

    showSpecificFields(0);
}

void EditItemWidget::visit(const Library::Movie& movie) {
    m_typeComboBox->setCurrentIndex(1);

    m_movieLanguageEdit->setText(QString::fromStdString(movie.getLanguage()));
    m_minAgeEdit->setValue(movie.getMinAge());
    m_durationMovieEdit->setValue(movie.getDuration());
    m_oscarCheckBox->setChecked(movie.getOscar());
    m_movieTrailerEdit->setText(QString::fromStdString(movie.getTrailer()));

    showSpecificFields(1);
}

void EditItemWidget::visit(const Library::CD& cd) {
    m_typeComboBox->setCurrentIndex(2);

    m_cdLanguageEdit->setText(QString::fromStdString(cd.getLanguage()));
    m_albumEdit->setText(QString::fromStdString(cd.getAlbum()));
    m_durationCDEdit->setValue(cd.getDuration());
    m_audioTrackEdit->setText(QString::fromStdString(cd.getaudioTrack()));

    showSpecificFields(2);
}

void EditItemWidget::onTypeChanged(int index) {
    if (index < 0 || index >= m_prototypes.size()) return;

    Library::Item* prototype = m_prototypes[index];
    prototype->accept(*this);
}

void EditItemWidget::showSpecificFields(int index) {
    m_bookFields->setVisible(index == 0);
    m_movieFields->setVisible(index == 1);
    m_cdFields->setVisible(index == 2);
}

void EditItemWidget::onSaveButtonClicked() {
    emit saveClicked();
}

void EditItemWidget::onCancelButtonClicked() {
    emit cancelClicked();
}
