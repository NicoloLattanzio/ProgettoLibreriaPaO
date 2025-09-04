#ifndef LISTITEMVISITOR_H
#define LISTITEMVISITOR_H

#include "model/Item.h"
#include "model/Visitor.h"
#include "QListWidgetItem"

class ListItemVisitor : public ItemsVisitor::ConstVisitor {
public:
    ListItemVisitor() = default;

    QListWidgetItem* getListWidgetItem() const { return m_listItem; }

    void visit(const Library::Book& book);
    void visit(const Library::Movie& movie);
    void visit(const Library::CD& cd);

private:
    QListWidgetItem* m_listItem;
};

#endif
