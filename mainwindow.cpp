#include "mainwindow.h"
#include "guivisitor.h"
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLineEdit>
#include <QListWidget>
#include <QVariant>

// necessario per QVariant con puntatori custom
Q_DECLARE_METATYPE(Library::Item*)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_dbManager(DatabaseManager::instance())
{
    setupUi();

    // Connessioni
    connect(searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);
    connect(itemsListWidget, &QListWidget::itemClicked,
            this, &MainWindow::onListItemClicked);

    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::showAddPage);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::DeleteOnClick);

    connect(m_addItemWidget, &ADDItemWidget::saveClicked, this, &MainWindow::saveNewItem);
    connect(m_addItemWidget, &ADDItemWidget::cancelClicked, this, &MainWindow::showListPage);

    connect(m_editItemWidget, &EditItemWidget::saveClicked, this, &MainWindow::saveEditedItem);
    connect(m_editItemWidget, &EditItemWidget::cancelClicked, this, &MainWindow::showListPage);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::ShowEditPage);


    // Caricamento dati dal DB
    if (!m_dbManager.initialize("library.db")) {
        QMessageBox::critical(this, "Database Error",
                              "Impossibile inizializzare il database!");
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

    // --- Controlli superiori ---
    QWidget* topControls = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topControls);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Cerca per titolo, autore o genere...");
    m_addButton = new QPushButton("Aggiungi Item", this);
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(m_addButton);
    mainLayout->addWidget(topControls);

    // --- Stack ---
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);

    // --- Pagina lista ---
    m_listPage = new QWidget(this);
    QHBoxLayout *listPageLayout = new QHBoxLayout(m_listPage);
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, m_listPage);

    // lista a sinistra
    itemsListWidget = new QListWidget(this);
    itemsListWidget->setMinimumWidth(300);
    mainSplitter->addWidget(itemsListWidget);

    // pannello di destra
    rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // layout per dettagli
    detailsLayout = new QVBoxLayout();
    QLabel* placeholder = new QLabel("Seleziona un item per vederne i dettagli");
    placeholder->setWordWrap(true);
    detailsLayout->addWidget(placeholder);

    rightLayout->addLayout(detailsLayout, 1);

    // pulsante modifica
    m_editButton = new QPushButton("Modifica Item", this);
    m_editButton->setEnabled(false);
    rightLayout->addWidget(m_editButton, 0, Qt::AlignLeft);

    // pulsante elimina
    m_deleteButton = new QPushButton("Elimina Item", this);
    m_deleteButton->setEnabled(false);
    rightLayout->addWidget(m_deleteButton, 0, Qt::AlignLeft);

    rightPanel->setLayout(rightLayout);

    mainSplitter->addWidget(rightPanel);
    QList<int> sizes; sizes << 300 << 600;
    mainSplitter->setSizes(sizes);

    listPageLayout->addWidget(mainSplitter);
    m_stackedWidget->addWidget(m_listPage);

    // --- Pagine aggiunte ---
    m_addItemWidget = new ADDItemWidget(this);
    m_stackedWidget->addWidget(m_addItemWidget);

    m_editItemWidget = new EditItemWidget(this);
    m_stackedWidget->addWidget(m_editItemWidget);
}

void MainWindow::showAddPage() {
    m_stackedWidget->setCurrentIndex(1);
    m_addButton->setEnabled(false);
}

void MainWindow::showListPage() {
    m_stackedWidget->setCurrentIndex(0);
    m_addButton->setEnabled(true);
}

void MainWindow::ShowEditPage() {
    QListWidgetItem* currentItem = itemsListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "Info", "Nessun elemento selezionato.");
        return;
    }

    Library::Item* libraryItem = currentItem->data(Qt::UserRole).value<Library::Item*>();
    if (!libraryItem) return;

    // Imposta l'item per la modifica
    m_editItemWidget->setItem(libraryItem);

    // Vai alla pagina di modifica (indice 2 nello stack)
    m_stackedWidget->setCurrentIndex(2);
    m_addButton->setEnabled(false);
}

// Aggiungi questo metodo per salvare le modifiche
void MainWindow::saveEditedItem() {
    Library::Item* updatedItem = m_editItemWidget->getUpdatedItem();

    if (updatedItem) {
        if (m_dbManager.saveItem(*updatedItem)) {
            QMessageBox::information(this, "Successo", "Item modificato correttamente.");

            // Ricarica gli item dal database
            loadItemsFromDatabase();

            // Torna alla pagina lista
            showListPage();
        } else {
            QMessageBox::warning(this, "Errore Database", "Impossibile salvare le modifiche.");
        }
        delete updatedItem;
    }
}


void MainWindow::saveNewItem() {
    Library::Item* newItem = m_addItemWidget->createItem();

    if (newItem) {
        if (m_dbManager.saveItem(*newItem)) {
            QMessageBox::information(this, "Successo", "Item aggiunto correttamente.");
            loadItemsFromDatabase();
            showListPage();
        } else {
            QMessageBox::warning(this, "Errore Database", "Impossibile salvare l'item.");
        }
        delete newItem;
    }
}

void MainWindow::onSearchTextChanged(const QString& text) {
    for (int i = 0; i < itemsListWidget->count(); ++i) {
        QListWidgetItem* item = itemsListWidget->item(i);
        Library::Item* libraryItem = item->data(Qt::UserRole).value<Library::Item*>();

        bool match = libraryItem->getTitle().find(text.toStdString()) != std::string::npos ||
                     libraryItem->getAuthor().find(text.toStdString()) != std::string::npos ||
                     libraryItem->getGenre().find(text.toStdString()) != std::string::npos;

        item->setHidden(!match);
    }
}

void MainWindow::loadItemsFromDatabase() {
    qDeleteAll(m_items);
    m_items.clear();
    m_items = m_dbManager.loadAllItems();
    populateListWidget();
}

void MainWindow::populateListWidget() {
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
    if (!libraryItem) return;

    GUIVisitor visitor;
    libraryItem->accept(visitor);

    // svuota pannello dettagli
    QLayoutItem* child;
    while ((child = detailsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    // aggiungi nuovo widget
    detailsLayout->addWidget(visitor.getResultWidget());

    m_editButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
}

void MainWindow::DeleteOnClick() {
    // Get the currently selected item
    QListWidgetItem* currentItem = itemsListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "Info", "Nessun elemento selezionato.");
        return;
    }

    // Get the Library::Item from the list item
    Library::Item* libraryItem = currentItem->data(Qt::UserRole).value<Library::Item*>();
    if (!libraryItem) {
        QMessageBox::warning(this, "Errore", "Impossibile ottenere i dati dell'elemento.");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Conferma eliminazione",
                                 "Sei sicuro di voler eliminare '" +
                                 QString::fromStdString(libraryItem->getTitle()) + "'?",
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Delete from database using the reference to the item
        if (m_dbManager.DeleteItem(*libraryItem)) {
            // Remove from our local list
            for (auto it = m_items.begin(); it != m_items.end(); ++it) {
                if (*it == libraryItem) {
                    delete *it; // Free the memory
                    m_items.erase(it);
                    break;
                }
            }

            // Remove from UI
            int row = itemsListWidget->row(currentItem);
            delete itemsListWidget->takeItem(row);

            // Clear details panel
            QLayoutItem* child;
            while ((child = detailsLayout->takeAt(0)) != nullptr) {
                if (child->widget()) child->widget()->deleteLater();
                delete child;
            }

            // Add placeholder text
            QLabel* placeholder = new QLabel("Seleziona un item per vederne i dettagli");
            placeholder->setWordWrap(true);
            detailsLayout->addWidget(placeholder);

            // Disable buttons
            m_editButton->setEnabled(false);
            m_deleteButton->setEnabled(false);

            QMessageBox::information(this, "Successo", "Elemento eliminato con successo.");
        } else {
            QMessageBox::warning(this, "Errore", "Impossibile eliminare l'elemento dal database.");
        }
    }
}
