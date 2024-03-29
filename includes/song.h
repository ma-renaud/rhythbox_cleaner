#ifndef RHYTHBOX_CLEANER_SONG_H
#define RHYTHBOX_CLEANER_SONG_H

#include "entry.h"

class Song : public Entry{
 public:
  Song();
  ~Song() = default;

  void set_property(const string &name, const string &value) override;

  Property<string> location;
  Property<string> title;
  Property<string> artist;
  Property<string> album;
  Property<int> duration{};
  Property<int> size{};
  Property<int> first_seen{};

  string url_to_path(const string &encoded);

  bool same_file(const Song& rhs) const;
  bool same_info(const Song& rhs) const;
  bool operator==(const Song& rhs) const;

};

#endif //RHYTHBOX_CLEANER_SONG_H