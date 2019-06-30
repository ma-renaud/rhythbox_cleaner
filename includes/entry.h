#ifndef RHYTHBOX_CLEANER_ENTRY_H
#define RHYTHBOX_CLEANER_ENTRY_H

#include <string>
#include <vector>
#include <optional>
#include "property.h"

using std::string;

class Entry {
 public:
  Entry() = default;
  ~Entry() = default;

  auto get_properties(){ return &properties; }
  virtual void set_property(const string &name, const string &value);

  static const std::string NOTSET;

 private:
  std::vector<std::pair<string, string>> properties;

};

#endif //RHYTHBOX_CLEANER_ENTRY_H
