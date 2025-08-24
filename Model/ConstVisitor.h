#ifndef CONSTVISITOR_H
#define CONSTVISITOR_H

namespace Library {
  class Book; class Movie; class CD;
}

namespace ItemsVisitor {
  class ConstVisitor {
    public:
      virtual ~ConstVisitor() = default;

      virtual void visit(const Library::Book& book) = 0;
      virtual void visit(const Library::Movie& movie) = 0;
      virtual void visit(const Library::CD& cd) = 0;

  };
}

#endif // CONSTVISITOR_H
