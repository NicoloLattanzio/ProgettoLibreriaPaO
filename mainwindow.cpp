#include "mainwindow.h"
#include "guivisitor.h"
#include "ADDItemWidget.h"
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QLineEdit>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_dbManager(DatabaseManager::instance())
{
    setupUi(); // Tutta la creazione della UI è ora in un metodo separato

    // Connessioni
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(itemsListWidget, &QListWidget::itemClicked, this, &MainWindow::onListItemClicked);

    // Il pulsante "Aggiungi" ora chiama lo slot per cambiare pagina
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::showAddPage);

    // Connettiamo i segnali del form di aggiunta agli slot di MainWindow
    connect(m_addItemWidget, &ADDItemWidget::saveClicked, this, &MainWindow::saveNewItem);
    connect(m_addItemWidget, &ADDItemWidget::cancelClicked, this, &MainWindow::showListPage);

    // Caricamento dati
    if (!m_dbManager.initialize("library.db")) {
        QMessageBox::critical(this, "Database Error", "Impossibile inizializzare il database!");
    } else {
        loadItemsFromDatabase();
    }
}

MainWindow::~MainWindow() {
    qDeleteAll(m_items);
}

void MainWindow::setupUi() {
    setWindowTitle("Library Manager");
    resize(900, 600);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Controlli superiori (sempre visibili) ---
    QWidget* topControls = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topControls);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Cerca per titolo, autore o genere...");
    m_addButton = new QPushButton("Aggiungi Item", this);
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(m_addButton);
    mainLayout->addWidget(topControls);

    // --- Creazione del QStackedWidget ---
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);

    // --- PAGINA 1: Lista degli Item ---
    m_listPage = new QWidget(this);
    QHBoxLayout *listPageLayout = new QHBoxLayout(m_listPage);
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, m_listPage);

    itemsListWidget = new QListWidget(this);
    itemsListWidget->setMinimumWidth(300);
    mainSplitter->addWidget(itemsListWidget);

    // Pannello di destra con dettagli e pulsante modifica
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    detailsLabel = new QLabel("Seleziona un item per vederne i dettagli", this);
    // ... (proprietà del detailsLabel)
    m_editButton = new QPushButton("Modifica Item", this);
    m_editButton->setEnabled(false);
    rightLayout->addWidget(detailsLabel, 1);
    rightLayout->addWidget(m_editButton, 0, Qt::AlignLeft);
    rightPanel->setLayout(rightLayout);

    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes({300, 600});
    listPageLayout->addWidget(mainSplitter);
    m_stackedWidget->addWidget(m_listPage); // Aggiunge la pagina della lista allo stack

    // --- PAGINA 2: Form di Aggiunta ---
    m_addItemWidget = new ADDItemWidget(this);
    m_stackedWidget->addWidget(m_addItemWidget); // Aggiunge la pagina del form allo stack
}


void MainWindow::showAddPage() {
    m_stackedWidget->setCurrentIndex(1); // Mostra la pagina di ADDItemWidget (indice 1)
    m_addButton->setEnabled(false); // Disabilita il pulsante "Aggiungi" mentre siamo già lì
}

void MainWindow::showListPage() {
    m_stackedWidget->setCurrentIndex(0); // Mostra la pagina della lista (indice 0)
    m_addButton->setEnabled(true); // Riabilita il pulsante
}

void MainWindow::saveNewItem() {
    Library::Item* newItem = m_addItemWidget->createItem();

    if (newItem) {
        if (m_dbManager.saveItem(*newItem)) {
            QMessageBox::information(this, "Successo", "Item aggiunto correttamente.");
            loadItemsFromDatabase();
            showListPage(); // Torna alla pagina della lista
        } else {
            QMessageBox::warning(this, "Errore Database", "Impossibile salvare l'item.");
        }
        delete newItem;
    }
}

// L'implementazione degli altri metodi (loadItemsFromDatabase, etc.) rimane invariata
void MainWindow::onSearchTextChanged(const QString& text) //Attenzione rimuovere il case sensitive
{
    for (int i = 0; i < itemsListWidget->count(); ++i) {
        QListWidgetItem* item = itemsListWidget->item(i);
        Library::Item* libraryItem = item->data(Qt::UserRole).value<Library::Item*>();

        bool match = libraryItem->getTitle().find(text.toStdString()) != std::string::npos ||
                     libraryItem->getAuthor().find(text.toStdString()) != std::string::npos ||
                     libraryItem->getGenre().find(text.toStdString()) != std::string::npos;

        item->setHidden(!match);
    }
}
void MainWindow::loadItemsFromDatabase() { qDeleteAll(m_items); m_items.clear(); m_items = m_dbManager.loadAllItems(); populateListWidget(); }
void MainWindow::populateListWidget()
{
    itemsListWidget->clear();

    for (Library::Item* item : m_items) {
        QString displayText = QString("%1\n%2 - %3")
            .arg(QString::fromStdString(item->getTitle()),
                 QString::fromStdString(item->getAuthor()),
                 QString::number(item->getYear()));

        QListWidgetItem* listItem = new QListWidgetItem(displayText);
        listItem->setData(Qt::UserRole, QVariant::fromValue(item));
        itemsListWidget->addItem(listItem);
    }
}
void MainWindow::onListItemClicked(QListWidgetItem* item) {
    Library::Item* libraryItem = item->data(Qt::UserRole).value<Library::Item*>();
    detailsLabel->setText(getItemDetails(libraryItem));
    m_editButton->setEnabled(true);
}

QString MainWindow::getItemDetails(Library::Item* item) {
    GUIVisitor visitor; item->accept(visitor);
    return visitor.result;
}
