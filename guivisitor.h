#ifndef GUIVISITOR_H
#define GUIVISITOR_H

#include "persistence/DatabaseManager.h"
#include "model/ConstVisitor.h"

class GUIVisitor : public ItemsVisitor::ConstVisitor {
public:
    QString result;

    void visit(const Library::Book& book) override;
    void visit(const Library::Movie& movie) override;
    void visit(const Library::CD& cd) override;
};


#endif // GUIVISITOR_H

