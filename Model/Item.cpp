#ifndef ITEM_CPP
#define ITEM_CPP

#include "Item.h"

namespace Library {

   Item::Item(String author, String title, unsigned int itemid, String genre, unsigned int year, String image):
       author(author) , title(title), itemid(itemid), genre(genre), year(year), image(image){}

   const String& Item::getAuthor() const {
     return author;
   }

  const String& Item::getTitle() const {
    return title;
  }

  unsigned int Item::getItemID() const {
    return itemid;
  }

  const String& Item::getGenre() const {
    return genre;
  }

  unsigned int Item::getYear() const {
    return year;
  }

  const String& Item::getImage() const {
    return image;
  }

  Item& Item::setAuthor(const String& author) {
    this->author = author;
    return *this;
  }

  Item& Item::setTitle(const String& title) {
    this->title = title;
    return *this;
  }

  Item& Item::setItemID(unsigned int itemid) {
    this->itemid = itemid;
    return *this;
  }

  Item& Item::setGenre(const String& genre) {
    this->genre = genre;
    return *this;
  }


  Item& Item::setYear(unsigned int year) {
    this->year = year;
    return *this;
  }

  Item& Item::setImage(const String& image) {
    this->image = image;
    return *this;
  }

}

#endif // ITEM_CPP
