#include "rhythmdb.h"

void RhythmDB::add_song(Song song) {
  songs.push_back(song);
}

void RhythmDB::add_podcast_feed(Entry entry) {
  podcast_feeds.push_back(entry);
}

void RhythmDB::add_podcast_post(Entry entry) {
  podcast_posts.push_back(entry);
}

void RhythmDB::add_radio(Entry entry) {
  radios.push_back(entry);
}

void RhythmDB::add_ignore(Song song) {
  ignores.push_back(song);
}
