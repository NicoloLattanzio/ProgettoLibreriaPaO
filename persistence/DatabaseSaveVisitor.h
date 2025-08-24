#ifndef DATABASESAVEVISITOR_H
#define DATABASESAVEVISITOR_H

#include "../model/Item.h"
#include "../model/ConstVisitor.h"
#include <QSqlQuery>

class DatabaseSaveVisitor : public ItemsVisitor::ConstVisitor {
public:
    explicit DatabaseSaveVisitor(int itemId);

    void visit(const Library::Book& book) override;
    void visit(const Library::Movie& movie) override;
    void visit(const Library::CD& cd) override;

private:
    int m_itemId;
};

#endif // DATABASESAVEVISITOR_H
