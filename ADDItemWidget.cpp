#include "ADDItemWidget.h"
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

ADDItemWidget::ADDItemWidget(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Aggiungi Nuovo Item");
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 1. Creazione UI
    m_typeComboBox = new QComboBox(this);
    m_typeComboBox->addItem("Libro");
    m_typeComboBox->addItem("Film");
    m_typeComboBox->addItem("CD");
    mainLayout->addWidget(m_typeComboBox);

    setupCommonFields();
    createSpecificFields();
    mainLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Salva", this);
    QPushButton* cancelButton = new QPushButton("Annulla", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // 2. Creazione Prototipi per il Visitor
    m_prototypes.append(new Library::Book());
    m_prototypes.append(new Library::Movie());
    m_prototypes.append(new Library::CD());

    // 3. Connessioni dei segnali
    connect(m_typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ADDItemWidget::onTypeChanged);
    connect(saveButton, &QPushButton::clicked, this, &ADDItemWidget::saveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ADDItemWidget::cancelClicked);

    // 4. Impostazione dello stato iniziale della UI
    onTypeChanged(0);
}

ADDItemWidget::~ADDItemWidget() {
    qDeleteAll(m_prototypes);
}

// --- Implementazione Metodi Visitor ---
void ADDItemWidget::visit(Library::Book& /*book*/) {
    showSpecificFields(0);
}

void ADDItemWidget::visit(Library::Movie& /*movie*/) {
    showSpecificFields(1);
}

void ADDItemWidget::visit(Library::CD& /*cd*/) {
    showSpecificFields(2);
}

void ADDItemWidget::onTypeChanged(int index) {
    if (index < 0 || index >= m_prototypes.size()) return;

    Library::Item* prototype = m_prototypes[index];
    prototype->accept(*this); // 'this' è il Visitor
}

void ADDItemWidget::showSpecificFields(int index) {
    m_bookFields->setVisible(index == 0);
    m_movieFields->setVisible(index == 1);
    m_cdFields->setVisible(index == 2);
}

// --- Implementazione Metodi Principali --- // RIVEDERE QUESTA FUNZIONE
Library::Item* ADDItemWidget::createItem() {
    if (m_titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Dati Mancanti", "Il campo Titolo è obbligatorio.");
        return nullptr;
    }

    int typeIndex = m_typeComboBox->currentIndex();
    String title = m_titleEdit->text().toStdString();
    String author = m_authorEdit->text().toStdString();
    String genre = m_genreEdit->text().toStdString();
    unsigned int year = m_yearEdit->value();
    String image = m_imageEdit->text().toStdString();

    if (typeIndex == 0) { // Libro
        return new Library::Book(author, title, 0, genre, year, image,
                                 m_publisherEdit->text().toStdString(),
                                 m_translatorEdit->text().toStdString(),
                                 m_pagesEdit->value());
    } else if (typeIndex == 1) { // Film
        return new Library::Movie(author, title, 0, genre, year, image,
                                  m_movieLanguageEdit->text().toStdString(),
                                  m_minAgeEdit->value(),
                                  m_durationMovieEdit->value(),
                                  m_oscarCheckBox->isChecked());
    } else if (typeIndex == 2) { // CD
        return new Library::CD(author, title, 0, genre, year, image,
                               m_cdLanguageEdit->text().toStdString(),
                               m_albumEdit->text().toStdString(),
                               m_durationCDEdit->value());
    }
    return nullptr;
}


// --- Implementazione Metodi di Setup UI ---
void ADDItemWidget::setupCommonFields() {
    QFormLayout *formLayout = new QFormLayout();
    m_titleEdit = new QLineEdit(this);
    m_authorEdit = new QLineEdit(this);
    m_genreEdit = new QLineEdit(this);
    m_yearEdit = new QSpinBox(this);
    m_imageEdit = new QLineEdit(this);

    m_yearEdit->setRange(1800, QDate::currentDate().year());
    m_yearEdit->setValue(QDate::currentDate().year());
    m_imageEdit->setPlaceholderText("URL o percorso file (opzionale)");

    formLayout->addRow("Titolo:", m_titleEdit);
    formLayout->addRow("Autore/Regista:", m_authorEdit);
    formLayout->addRow("Genere:", m_genreEdit);
    formLayout->addRow("Anno:", m_yearEdit);
    formLayout->addRow("Immagine:", m_imageEdit);

    static_cast<QVBoxLayout*>(layout())->addLayout(formLayout);
}

void ADDItemWidget::createSpecificFields() {
    QVBoxLayout* mainLayout = static_cast<QVBoxLayout*>(layout());

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

    m_movieFields = new QWidget(this);
    QFormLayout* movieLayout = new QFormLayout(m_movieFields);
    m_movieLanguageEdit = new QLineEdit(m_movieFields);
    m_minAgeEdit = new QSpinBox(m_movieFields);
    m_durationMovieEdit = new QSpinBox(m_movieFields);
    m_oscarCheckBox = new QCheckBox(m_movieFields);
    m_durationMovieEdit->setRange(1, 600);
    movieLayout->addRow("Lingua:", m_movieLanguageEdit);
    movieLayout->addRow("Età Minima:", m_minAgeEdit);
    movieLayout->addRow("Durata (min):", m_durationMovieEdit);
    movieLayout->addRow("Vincitore Oscar:", m_oscarCheckBox);
    mainLayout->addWidget(m_movieFields);

    m_cdFields = new QWidget(this);
    QFormLayout* cdLayout = new QFormLayout(m_cdFields);
    m_cdLanguageEdit = new QLineEdit(m_cdFields);
    m_albumEdit = new QLineEdit(m_cdFields);
    m_durationCDEdit = new QSpinBox(m_cdFields);
    m_durationCDEdit->setRange(1, 300);
    cdLayout->addRow("Lingua:", m_cdLanguageEdit);
    cdLayout->addRow("Album:", m_albumEdit);
    cdLayout->addRow("Durata (min):", m_durationCDEdit);
    mainLayout->addWidget(m_cdFields);
}
