#ifndef BOOK_H
#define BOOK_H

#include "Item.h"

namespace Library {

  class Book : public Item {

    private:
      String publisher;
      String translator;
      unsigned int pages;

    public:
      Book(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
           String publisher,String translator, unsigned int pages);
      Book() = default;

      virtual void accept(ItemsVisitor::Visitor& visitor) override;
      virtual void accept(ItemsVisitor::ConstVisitor& constVisitor) const override;

      const String& getPublisher() const;
      const String& getTranslator() const;
      unsigned int getPages() const;

      Book& setPublisher(const String& publisher);
      Book& setTranslator(const String& translator);
      Book& setPages(unsigned int pages);



  };

}
#endif // BOOK_H
