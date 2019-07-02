#include "appcli.h"

#include <thread>
#include <functional>
#include <fstream>
#include <boost/filesystem.hpp>

#include "mxml.h"
#include "classwithcallback.h"
#include "rhythmdb.h"
#include "displaythread.h"

namespace fs = boost::filesystem;

void AppCLI::run() {
  RhythmDB rhythmdb;
  parser = std::make_unique<RhythmDBXML>(&rhythmdb);

  auto clean_request_callback = std::bind(&AppCLI::answer_clean_request, this, std::placeholders::_1);
  menu = std::make_unique<MenuCLI>(&rhythmdb);

  DisplayThread display(menu.get());
  auto display_run = std::bind(&DisplayThread::run, &display);
  std::thread threadObj(display_run);

  fs::path home = fs::path(getenv("HOME"));
  create_backup(home.string() + "/.local/share/rhythmbox/rhythmdb.xml",
                home.string() + "/.local/share/rhythmbox/rhythmdb.xml.bak");

  if (load_database(home.string() + "/.local/share/rhythmbox/rhythmdb.xml")) {
    menu->start_scan();
    rhythmdb.search_problems();
    menu->scan_completed(clean_request_callback);

    if (clean_requested) {
      menu->start_fix();
      rhythmdb.fix_problems();
      menu->fix_completed();
    }

    write_database_to_file(home.string() + "/.local/share/rhythmbox/rhythmdb.xml");
  }

  display.stop();
  if (threadObj.joinable())
    threadObj.join();
}

void AppCLI::answer_clean_request(bool answer) {
  clean_requested = answer;
}

void AppCLI::create_backup(const std::string& src_path, const std::string& dest_path) {
  std::ifstream src(src_path, std::ios::binary);
  std::ofstream dst(dest_path, std::ios::binary);
  dst << src.rdbuf();
}

bool AppCLI::load_database(const std::string &database_path) {
  FILE *fp;
  fp = fopen(database_path.c_str(), "r");
  if (fp) {
    menu->start_load();
    mxmlSAXLoadFile(nullptr,
                    fp,
                    MXML_TEXT_CALLBACK,
                    SaxMemberFunctionCallback(parser.get(), &RhythmDBXML::sax_callback),
                    nullptr);
    fclose(fp);
    menu->load_completed();
    return true;
  } else
    return false;
}

void AppCLI::write_database_to_file(const std::string &database_path) {
  parser->write_to_file(database_path);

  std::ifstream src(database_path, std::ios::binary);
  std::string file((std::istreambuf_iterator<char>(src)), std::istreambuf_iterator<char>());
  src.close();

  file.insert(36, " standalone=\"yes\"");

  std::ofstream dest(database_path);
  dest << file;
  dest.close();
}
