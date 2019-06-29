#include "rhythmdb.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <boost/filesystem.hpp>

void RhythmDB::search_problems() {
  std::deque<std::pair<Song *, Song *>> conflicts;
  int nb_songs = songs.size();
  int current_song = 0;

  while (current_song < nb_songs - 1) {
    for (int i = current_song + 1; i < nb_songs; i++) {
      if (songs[current_song].same_info(songs[i]))
        conflicts.emplace_back(&songs[current_song], &songs[i]);
    }
    current_song++;
  }

  for (auto &each : conflicts) {
    if (each.first->same_file(*each.second))
      duplicated_songs_same_file.emplace_back(each.first, each.second);
    else
      duplicated_songs_same_info.emplace_back(each.first, each.second);
  }
}

void RhythmDB::add_song(const Song &song) {
  songs.push_back(song);
}

void RhythmDB::add_podcast_feed(const Entry &entry) {
  podcast_feeds.push_back(entry);
}

void RhythmDB::add_podcast_post(const Entry &entry) {
  podcast_posts.push_back(entry);
}

void RhythmDB::add_radio(const Entry &entry) {
  radios.push_back(entry);
}

void RhythmDB::add_ignore(const Song &song) {
  ignores.push_back(song);
}
