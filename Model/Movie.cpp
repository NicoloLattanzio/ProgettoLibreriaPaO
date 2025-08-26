
#include "Movie.h"

namespace Library {

   Movie::Movie(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
              String language, unsigned int MinAge, unsigned int duration,bool oscar, String trailer):Item(author, title, itemid, genre, year,
                                                                            image), language(language), MinAge(MinAge), duration(duration), oscar(oscar), trailer(trailer)  {}



   const String& Movie::getLanguage() const {
     return language;
   }

  unsigned int Movie::getMinAge() const {
    return MinAge;
  }

  unsigned int Movie::getDuration() const {
    return duration;
  }
  bool Movie::getOscar() const {
    return oscar;
  }

  const String& Movie::getTrailer() const {
    return trailer;
  }

  Movie& Movie::setLanguage(const String& language) {
    this->language = language;
    return *this;
  }

  Movie& Movie::setMinAge(unsigned int MinAge) {
    this->MinAge = MinAge;
    return *this;
  }

  Movie& Movie::setDuration(unsigned int duration) {
    this->duration = duration;
    return *this;
  }

  Movie& Movie::setOscar(bool oscar) {
    this->oscar = oscar;
    return *this;
  }

  Movie& Movie::setTrailer(const String& trailer) {
    this->trailer = trailer;
    return *this;
  }

  void Movie::accept(ItemsVisitor::Visitor& visitor) {
    visitor.visit(*this);
  }

  void Movie::accept(ItemsVisitor::ConstVisitor& constVisitor) const {
    constVisitor.visit(*this);
  }

}
