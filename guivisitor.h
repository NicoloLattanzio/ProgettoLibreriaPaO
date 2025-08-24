#ifndef GUIVISITOR_H
#define GUIVISITOR_H

#include "model/ConstVisitor.h"
#include "model/Book.h"
#include "model/Movie.h"
#include "model/CD.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QMediaPlayer>

class GUIVisitor : public ItemsVisitor::ConstVisitor {
public:

    GUIVisitor();


    void visit(const Library::Book& book);
    void visit(const Library::Movie& movie);
    void visit(const Library::CD& cd);
    void clear();
    QWidget* getResultWidget() const;

private:
    QWidget* resultWidget;
    QVBoxLayout* mainLayout;
    QMediaPlayer *player;
    ;
};

#endif // GUIVISITOR_H
