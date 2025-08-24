#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "persistence/DatabaseManager.h"
#include "ADDItemWidget.h"
#include "model/Item.h"
#include "edititemwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAddPage();
    void showListPage();
    void saveNewItem();
    void onSearchTextChanged(const QString& text);
    void onListItemClicked(QListWidgetItem* item);
    void DeleteOnClick();
    void ShowEditPage();
    void saveEditedItem();

private:
    void setupUi();
    void loadItemsFromDatabase();
    void populateListWidget();


    DatabaseManager& m_dbManager;
    QList<Library::Item*> m_items;

    QLineEdit* searchLineEdit;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;

    QStackedWidget* m_stackedWidget;
    QWidget* m_listPage;
    ADDItemWidget* m_addItemWidget;
    QListWidget* itemsListWidget;
    EditItemWidget* m_editItemWidget;

    // pannello di destra
    QWidget* rightPanel;
    QVBoxLayout* detailsLayout;
};

#endif // MAINWINDOW_H
