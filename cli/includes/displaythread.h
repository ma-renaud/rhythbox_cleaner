#ifndef DISPLAYTHREAD_H
#define DISPLAYTHREAD_H

#include <thread>
#include <mutex>
#include "menucli.h"

class DisplayThread {
 public:
  explicit DisplayThread(MenuCLI *menu) : menu(menu) {}
  ~DisplayThread() = default;

  void stop() { active = false; }

  void run() {
    while (active) {
      menu->show_menu();
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }

 private:
  MenuCLI *menu;
  bool active {true};
};

#endif //DISPLAYTHREAD_H
