#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "persistence/DatabaseManager.h"

// Forward declarations
class QListWidget;
class QLabel;
class QLineEdit;
class QSplitter;
class QPushButton;
class QListWidgetItem;
class QStackedWidget; // Aggiunto
class ADDItemWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchTextChanged(const QString& text);
    void onListItemClicked(QListWidgetItem* item);

    // Slot per la nuova logica
    void showAddPage();
    void showListPage();
    void saveNewItem();

private:
    void setupUi();
    void loadItemsFromDatabase();
    void populateListWidget();
    QString getItemDetails(Library::Item* item);

    DatabaseManager& m_dbManager;
    QVector<Library::Item*> m_items;

    // Widget della UI
    QStackedWidget *m_stackedWidget; // Il nostro contenitore di pagine
    QWidget *m_listPage;             // La pagina con la lista
    ADDItemWidget *m_addItemWidget;  // La pagina di aggiunta

    QListWidget *itemsListWidget;
    QLabel *detailsLabel;
    QLineEdit *searchLineEdit;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
};

#endif // MAINWINDOW_H
