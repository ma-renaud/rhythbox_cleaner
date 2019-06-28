#include "rhythmdbxml.h"

void RhythmDBXML::write_to_file(const std::string& path) {
  mxml_node_t *xml;       /*<?xml ... ?> */
  mxml_node_t *data;

  xml = mxmlNewXML("1.0");

  data = mxmlNewElement(xml, "rhythmdb");
  mxmlElementSetAttr(data, "version", rhythmdb->version().c_str());

  for (auto &each : *(rhythmdb->get_podcast_feeds())) {
    write_entry(&each, "podcast-feed", data);
  }

  for (auto &each : *(rhythmdb->get_radios())) {
    write_entry(&each, "iradio", data);
  }

  for (auto &each : *(rhythmdb->get_ignores())) {
    write_entry(&each, "ignore", data);
  }

  for (auto &each : *(rhythmdb->get_podcast_posts())) {
    write_entry(&each, "podcast-post", data);
  }

  for (auto &each : *(rhythmdb->get_songs())) {
    write_entry(&each, "song", data);
  }

  FILE *fp;
  fp = fopen(path.c_str(), "w");
  mxmlSaveFile(xml, fp, whitespace_cb);
  fclose(fp);
}

void RhythmDBXML::sax_callback(mxml_node_t *node, mxml_sax_event_t event, void *data) {
  std::ignore = data;
  if (event != MXML_SAX_ELEMENT_CLOSE && current_node != NONE)
    mxmlRetain(node);

  if (event == MXML_SAX_ELEMENT_OPEN) {
    std::string node_name = node->value.element.name;
    if (node_name == "entry") {
      entry_open(string(mxmlElementGetAttr(node, "type")));
    } else if (node_name == "rhythmdb") {
      rhythmdb->version = string(mxmlElementGetAttr(node, "version"));
    }
  } else if (event == MXML_SAX_ELEMENT_CLOSE) {
    std::string node_name = node->value.element.name;
    if (node_name == "entry") {
      entry_close(node);
    }
  }
}

void RhythmDBXML::entry_open(std::string type) {
  if (type == "song") {
    current_node = SONG;
    entry = std::make_unique<Song>();
  }
  else if (type == "podcast-feed") {
    current_node = PODCAST_FEED;
    entry = std::make_unique<Entry>();
  }
  else if (type == "iradio") {
    current_node = IRADIOS;
    entry = std::make_unique<Entry>();
  }
  else if (type == "ignore") {
    current_node = IGNORES;
    entry = std::make_unique<Song>();
  }
  else if (type == "podcast-post") {
    current_node = PODCAST_POST;
    entry = std::make_unique<Entry>();
  }
}

void RhythmDBXML::entry_close(mxml_node_t *node) {
  switch(current_node){
    case SONG:
      extract_entry_data(node);
      rhythmdb->add_song(*(dynamic_cast<Song*>((entry.get()))));
      break;
    case PODCAST_FEED:
      extract_entry_data(node);
      rhythmdb->add_podcast_feed(*(entry.get()));
      break;
    case IRADIOS:
      extract_entry_data(node);
      rhythmdb->add_radio(*(entry.get()));
      break;
    case IGNORES:
      extract_entry_data(node);
      rhythmdb->add_ignore(*(dynamic_cast<Song*>((entry.get()))));
      break;
    case PODCAST_POST:
      extract_entry_data(node);
      rhythmdb->add_podcast_post(*(entry.get()));
    default:
      break;
  }
  current_node = NONE;
}

void RhythmDBXML::extract_entry_data(mxml_node_t *node) {
  mxml_node_t *data = node->child;
  while (data) {
    if (data->last_child || data->type == MXML_ELEMENT) {
      entry->set_property(data->value.element.name, extract_value(data->child));
    }
    data = data->next;
  }
}

std::string RhythmDBXML::extract_value(mxml_node_t *node) {
  std::string value_str;
  while (node) {
    for (int i = 0; i < node->value.text.whitespace; i++)
      value_str.append(" ");
    value_str.append(node->value.text.string);
    node = node->next;
  }
  return value_str;
}

void RhythmDBXML::write_entry(Entry *entry, std::string type, mxml_node_t *parent) {
  mxml_node_t *node;
  mxml_node_t *entry_node;
  entry_node = mxmlNewElement(parent, "entry");
  mxmlElementSetAttr(entry_node, "type", type.c_str());
  for (auto const&[key, val] : *(entry->get_properties())) {
    node = mxmlNewElement(entry_node, key.c_str());
    mxmlNewText(node, 0, val.c_str());
  }
}

const char *RhythmDBXML::whitespace_cb(mxml_node_t *node, int where) {
  const char *name;
  static int indent = 0;
  std::string res;

  name = node->value.element.name;

  if (!strcmp(name, "rhythmdb") || !strcmp(name, "entry") || indent > 0) {
    if (!strcmp(name, "rhythmdb") || !strcmp(name, "entry")) {
      if (where == MXML_WS_BEFORE_CLOSE)
        indent--;
    }

    if (where == MXML_WS_BEFORE_OPEN)
      res.append("\n");
    if (where == MXML_WS_BEFORE_CLOSE && (!strcmp(name, "rhythmdb") || !strcmp(name, "entry")))
      res.append("\n");
    if (where == MXML_WS_BEFORE_OPEN && indent > 0)
      for (int i = 0; i < indent; i++)
        res.append("  ");
    if (where == MXML_WS_BEFORE_CLOSE && indent > 0 && (!strcmp(name, "rhythmdb") || !strcmp(name, "entry")))
      for (int i = 0; i < indent; i++)
        res.append("  ");

    if (!strcmp(name, "rhythmdb") || !strcmp(name, "entry")) {
      if (where == MXML_WS_BEFORE_OPEN)
        indent++;
    }
    char char_array[res.length() + 1];
    strcpy(char_array, res.c_str());
    return res.c_str();
  }

  return nullptr;
}
