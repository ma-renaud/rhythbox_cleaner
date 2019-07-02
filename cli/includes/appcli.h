#ifndef APPCLI_H
#define APPCLI_H

#include <string>
#include <memory>
#include "menucli.h"
#include "rhythmdbxml.h"

class AppCLI {
 public:
  AppCLI() = default;
  ~AppCLI() = default;

  void run();
  void answer_clean_request(bool answer);

 private:
  bool clean_requested {false};
  std::unique_ptr<MenuCLI> menu;
  std::unique_ptr<RhythmDBXML> parser;

  void create_backup(const std::string& src_path, const std::string& dest_path);
  bool load_database(const std::string& database_path);
  void write_database_to_file(const std::string& database_path);
};

#endif //APPCLI_H
