#ifndef MOVIE_H
#define MOVIE_H
#include "Item.h"

namespace Library {

  class Movie : public Item {

    private:
      String language;
      unsigned int MinAge;
      unsigned int duration;
      bool oscar;
      String trailer;

    public:
      Movie(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
           String language,unsigned int MinAge, unsigned int duration, bool oscar, String trailer);
      Movie() = default;
      virtual void accept(ItemsVisitor::Visitor& visitor) override;
      virtual void accept(ItemsVisitor::ConstVisitor& constVisitor) const override;

      const String& getLanguage() const;
      unsigned int getMinAge() const;
      unsigned int getDuration() const;
      bool getOscar() const;
      const String& getTrailer() const;

      Movie& setLanguage(const String& language);
      Movie& setMinAge(unsigned int album);
      Movie& setDuration(unsigned int duration);
      Movie& setOscar(bool oscar);
      Movie& setTrailer(const String& trailer);

  };

}
#endif // MOVIE_H
