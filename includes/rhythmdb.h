#ifndef RHYTHBOX_CLEANER_RHYTHMDB_H
#define RHYTHBOX_CLEANER_RHYTHMDB_H

#include <list>
#include "song.h"
#include "property.h"

class RhythmDB {
 public:
  RhythmDB() { version = "2.0"; }
  ~RhythmDB() = default;

  void add_song(Song song);
  void add_podcast_feed(Entry entry);
  void add_podcast_post(Entry entry);
  void add_radio(Entry entry);
  void add_ignore(Song song);

  auto get_songs() { return &songs; }
  auto get_podcast_feeds() { return &podcast_feeds; }
  auto get_podcast_posts() { return &podcast_posts; }
  auto get_radios() { return &radios; }
  auto get_ignores() { return &ignores; }

  int nb_podcast_feeds() { return podcast_feeds.size(); }
  int nb_radios() { return radios.size(); }
  int nb_ignores() { return ignores.size(); }
  int nb_podcast_posts() { return podcast_posts.size(); }
  int nb_songs() { return songs.size(); }

  Property<std::string> version;

 private:
  std::list<Entry> podcast_feeds;
  std::list<Entry> radios;
  std::list<Song> ignores;
  std::list<Entry> podcast_posts;
  std::list<Song> songs;

};

#endif //RHYTHBOX_CLEANER_RHYTHMDB_H
