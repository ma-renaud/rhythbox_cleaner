#include "song.h"
#include <boost/filesystem.hpp>
#include <curl/curl.h>

namespace fs = boost::filesystem;

Song::Song() {
  location = Entry::NOTSET;
  file = Entry::NOTSET;
  title = Entry::NOTSET;
  artist = Entry::NOTSET;
  album = Entry::NOTSET;
  duration = 0;
  size = 0;
}

void Song::set_property(string name, string value) {
  Entry::set_property(name, value);

  if (name == "title")
    title = value;
  if (name == "artist")
    artist = value;
  if (name == "album")
    album = value;
  if (name == "duration")
    duration = std::stoi(value);
  if (name == "file-size")
    size = std::stoi(value);
  if (name == "location") {
    location = url_to_path(value.substr(7));
  }
}

string Song::url_to_path(const string &encoded) {
  CURL *curl = curl_easy_init();
  int outlength;
  char *cres = curl_easy_unescape(curl, encoded.c_str(), encoded.length(), &outlength);
  string res(cres, cres + outlength);
  curl_free(cres);
  curl_easy_cleanup(curl);
  return res;
}

bool Song::same_file(const Song &rhs) const {
  try {
    fs::path p1{location()}, p2{rhs.location()};
    if (fs::equivalent(p1, p2))
      return true;
  }
  catch (const fs::filesystem_error &e) {
    return false;
  }
  return false;
}

bool Song::same_info(const Song &rhs) const {
  bool res = false;

  if (title() == rhs.title() && artist() == rhs.artist() &&
      album() == rhs.album() && duration() == rhs.duration() && size() == rhs.size())
    res = true;

  return res;
}

bool Song::operator==(const Song &rhs) const {
  bool res = same_info(rhs);
  if (res)
    res = same_file(rhs);
  return res;
}