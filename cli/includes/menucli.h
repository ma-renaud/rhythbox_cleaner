#ifndef MENUCLI_H
#define MENUCLI_H

#include <memory>
#include <mutex>
#include <functional>
#include "progressbar.h"

class RhythmDB;

class MenuCLI {
 public:
  explicit MenuCLI(RhythmDB *db) : rhythmdb(db) {}
  ~MenuCLI() = default;

  void show_menu();
  void start_load();
  void load_completed();
  void start_scan();
  void scan_completed(const std::function<void (bool)>& callback);
  void start_fix();
  void fix_completed();

 private:
  enum MenuState {
    LOAD,
    SCAN,
    FIX,
    NONE
  };

  unsigned int current_scan_count = 0;
  unsigned int current_fix_count = 0;
  RhythmDB *rhythmdb;
  MenuState current_state {NONE};
  std::unique_ptr<ProgressBar> progressBar;
  std::mutex mutex;

  char cursor[4] = {'/', '-', '\\', '|'};

  void show_load();
  void show_scan();
  void show_fix();
};

#endif //MENUCLI_H
