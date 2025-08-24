#ifndef VISITOR_H
#define VISITOR_H

namespace Library {
    class Book;
    class Movie;
    class CD;
}

namespace ItemsVisitor {
    class Visitor {
    public:
        virtual ~Visitor() = default;

        virtual void visit(Library::Book& book) = 0;
        virtual void visit(Library::Movie& movie) = 0;
        virtual void visit(Library::CD& cd) = 0;
    };
}

#endif // VISITOR_H
