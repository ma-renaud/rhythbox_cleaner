#include "rhythmdb.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <boost/filesystem.hpp>

void RhythmDB::search_problems() {
  scan_progress = 0;
  std::deque<std::pair<Song, Song>> conflicts;
  int nb_songs = songs.size();
  int current_song = 0;

  while (current_song < nb_songs - 1) {
    for (int i = current_song + 1; i < nb_songs; i++) {
      if (songs[current_song].same_info(songs[i]))
        conflicts.emplace_back(songs[current_song], songs[i]);
      scan_progress = scan_progress() + 1;
    }
    current_song++;
  }

  for (auto &each : conflicts) {
    if (each.first.same_file(each.second))
      duplicated_songs_same_file.emplace_back(each.first, each.second);
    else
      duplicated_songs_same_info.emplace_back(each.first, each.second);
  }
}

void RhythmDB::fix_problems() {
  fix_progress = 0;
  Song* to_delete;
  for (auto &each : duplicated_songs_same_file) {
    if (each.first.first_seen() > each.second.first_seen())
      to_delete = &each.first;
    else
      to_delete = &each.second;

    if (to_delete) {
      auto res = std::find(songs.begin(), songs.end(), *to_delete);
      if (res != songs.end())
        songs.erase(res);
    }
    fix_progress = fix_progress() + 1;
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
