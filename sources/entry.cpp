#include "entry.h"
#include <algorithm>

const std::string Entry::NOTSET = "__notset__";

void Entry::set_property(const string &name, const string &value) {
  properties.emplace_back(name, value);
}
