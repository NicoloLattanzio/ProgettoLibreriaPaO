
#include "CD.h"

namespace Library {

   CD::CD(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
              String language, String album, unsigned int duration):Item(author, title, itemid, genre, year,
                                                                            image), language(language), album(album), duration(duration)  {}



   const String& CD::getLanguage() const {
     return language;
   }

  const String& CD::getAlbum() const {
    return album;
  }

  unsigned int CD::getDuration() const {
    return duration;
  }

  CD& CD::setLanguage(const String& language) {
    this->language = language;
    return *this;
  }

  CD& CD::setAlbum(const String& album) {
    this->album = album;
    return *this;
  }

  CD& CD::setDuration(unsigned int duration) {
    this->duration = duration;
    return *this;
  }


  void CD::accept(ItemsVisitor::Visitor& visitor) {
    visitor.visit(*this);
  }

  void CD::accept(ItemsVisitor::ConstVisitor& constVisitor) const {
    constVisitor.visit(*this);
  }

}
