#ifndef RHYTHBOX_CLEANER_RHYTHMDB_H
#define RHYTHBOX_CLEANER_RHYTHMDB_H

#include <deque>
#include "song.h"
#include "property.h"

class RhythmDB {
 public:
  RhythmDB() { version = "2.0"; scan_progress = 0; fix_progress = 0; }
  ~RhythmDB() = default;

  void search_problems();
  void fix_problems();

  void add_song(const Song &song);
  void add_podcast_feed(const Entry &entry);
  void add_podcast_post(const Entry &entry);
  void add_radio(const Entry &entry);
  void add_ignore(const Song &song);

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

  int nb_same_file() { return duplicated_songs_same_file.size(); }
  int nb_same_info_only() { return duplicated_songs_same_info.size(); }

  Property<std::string> version;
  Property<unsigned int> scan_progress{};
  Property<unsigned int> fix_progress{};

 private:
  std::deque<Entry> podcast_feeds;
  std::deque<Entry> radios;
  std::deque<Song> ignores;
  std::deque<Entry> podcast_posts;
  std::deque<Song> songs;
  std::deque<std::pair<Song, Song>> duplicated_songs_same_file;
  std::deque<std::pair<Song, Song>> duplicated_songs_same_info;

};

#endif //RHYTHBOX_CLEANER_RHYTHMDB_H
