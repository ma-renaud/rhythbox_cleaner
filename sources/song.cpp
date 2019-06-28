#include "song.h"
#include <algorithm>

Song::Song() {
  location = Entry::NOTSET;
}

void Song::set_property(string name, string value) {
  Entry::set_property(name, value);
  if (name == "location")
    location = value;
}
