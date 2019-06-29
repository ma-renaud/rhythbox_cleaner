#ifndef RHYTHBOX_CLEANER_RHYTHMDBXML_H
#define RHYTHBOX_CLEANER_RHYTHMDBXML_H

#include <unordered_set>
#include <string>
#include <memory>
#include "mxml.h"
#include "rhythmdb.h"
class RhythmDBXML {
 public:
  explicit RhythmDBXML(RhythmDB *db): rhythmdb(db) {}
  ~RhythmDBXML() = default;

  void write_to_file(const std::string& path);
  void sax_callback(mxml_node_t *node, mxml_sax_event_t event, void *data);

  static const char*  whitespace_cb(mxml_node_t *node, int where);

 private:
  enum Nodes {
    NONE,
    PODCAST_FEED,
    IRADIOS,
    IGNORES,
    PODCAST_POST,
    SONG,
  };

  RhythmDB *rhythmdb;

  Nodes current_node = NONE;
  std::unique_ptr<Entry> entry;

  void entry_open(const std::string& type);
  void entry_close(mxml_node_t *node);
  void extract_entry_data(mxml_node_t *node);
  static std::string extract_value(mxml_node_t *node);
  void write_entry(Entry *_entry, const std::string& type, mxml_node_t *parent);
};

#endif //RHYTHBOX_CLEANER_RHYTHMDBXML_H
