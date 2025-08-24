#ifndef CD_H
#define CD_H
#include "Item.h"

namespace Library {

  class CD : public Item {

    private:
      String language;
      String album;
      unsigned int duration;

    public:
      CD() = default;

      CD(String author, String title, unsigned int itemid, String genre, unsigned int year, String image,
           String language,String album, unsigned int duration);

      virtual void accept(ItemsVisitor::Visitor& visitor) override;
      virtual void accept(ItemsVisitor::ConstVisitor& constVisitor) const override;

      const String& getLanguage() const;
      const String& getAlbum() const;
      unsigned int getDuration() const;

      CD& setLanguage(const String& language);
      CD& setAlbum(const String& album);
      CD& setDuration(unsigned int duration);

  };

}

#endif // CD_H
