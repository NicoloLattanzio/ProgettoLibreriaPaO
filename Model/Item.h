#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "./Visitor.h"
#include "./ConstVisitor.h"

namespace Library {

  using String = std::string;

  class Item {

    private:
      String author;
      String title;
      unsigned int itemid;
      String genre;
      unsigned int year;
      String image;



    public:
      Item() = default;
      Item(String Author, String title, unsigned int ItemID, String genre, unsigned int year, String image);
      virtual ~Item() = default;

      virtual void accept(ItemsVisitor::Visitor& visitor) = 0;
      virtual void accept(ItemsVisitor::ConstVisitor& visitor) const = 0;

      const String& getAuthor() const;
      const String& getTitle() const;
      unsigned int getItemID() const;
      const String& getGenre() const;
      unsigned int getYear() const;
      const String& getImage() const;

      Item& setAuthor(const String& author);
      Item& setTitle(const String& title);
      Item& setItemID(unsigned int itemid);
      Item& setGenre(const String& genre);
      Item& setYear(unsigned int year);
      Item& setImage(const String& image);

  };
}


#endif // ITEM_H
