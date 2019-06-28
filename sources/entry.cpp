#include "entry.h"
#include <algorithm>

const std::string Entry::NOTSET = "__notset__";

void Entry::set_property(string name, string value) {
  properties.emplace_back(name, value);
}

std::optional<std::string> Entry::get_property(string name) {
  auto res = std::find_if(properties.begin(), properties.end(), [&name](const std::pair<string, string>& x) { return x.first == name; });
  if (res != properties.end())
    return res->second;
  else
    return std::nullopt;
}
