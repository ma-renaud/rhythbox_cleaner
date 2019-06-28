#ifndef RHYTHBOX_CLEANER_SONG_H
#define RHYTHBOX_CLEANER_SONG_H

#include "entry.h"

class Song : public Entry{
 public:
  Song();
  ~Song() = default;

  void set_property(string name, string value) override;

  Property<string> location;

};

#endif //RHYTHBOX_CLEANER_SONG_H
