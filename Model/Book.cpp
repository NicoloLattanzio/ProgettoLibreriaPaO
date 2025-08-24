
#include "Book.h"

namespace Library {

   Book::Book(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
              String publisher, String translator, unsigned int pages):Item(author, title, itemid, genre, year,
                                                                            image), publisher(publisher), translator(translator), pages(pages)  {}



   const String& Book::getPublisher() const {
     return publisher;
   }

  const String& Book::getTranslator() const {
    return translator;
  }

  unsigned int Book::getPages() const {
    return pages;
  }

  Book& Book::setPublisher(const String& publisher) {
    this->publisher = publisher;
    return *this;
  }

  Book& Book::setTranslator(const String& translator) {
    this->translator = translator;
    return *this;
  }

  Book& Book::setPages(unsigned int pages) {
    this->pages = pages;
    return *this;
  }

  void Book::accept(ItemsVisitor::Visitor& visitor) {
    visitor.visit(*this);
  }

  void Book::accept(ItemsVisitor::ConstVisitor& constVisitor) const {
    constVisitor.visit(*this);
  }

}
