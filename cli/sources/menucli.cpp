#include "menucli.h"

#include <iostream>
#include "rhythmdb.h"

void MenuCLI::show_menu() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  switch (current_state) {
    case LOAD:
      show_load();
      break;
    case SCAN:
      show_scan();
      break;
    case FIX:
      show_fix();
      break;
    default:
      break;
  }
}

void MenuCLI::start_load() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_state = LOAD;
}

void MenuCLI::load_completed() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_state = NONE;
  std::cout << "Rhythmbox Database Loaded                " << std::endl;

  std::cout << "Nb songs: " << rhythmdb->nb_songs() << std::endl;
  std::cout << "Nb radios: " << rhythmdb->nb_radios() << std::endl;
  std::cout << "Nb ignores: " << rhythmdb->nb_ignores() << std::endl;
  std::cout << "Nb podcast feeds: " << rhythmdb->nb_podcast_feeds() << std::endl;
  std::cout << "Nb podcast posts: " << rhythmdb->nb_podcast_posts() << std::endl;
  std::cout<<std::endl;
}

void MenuCLI::start_scan() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_scan_count = 0;
  int nb_songs = rhythmdb->nb_songs() - 1;
  unsigned int total = ((nb_songs * nb_songs) + nb_songs) / 2;

  progressBar = std::make_unique<ProgressBar>(total, 70);
  current_state = SCAN;

  std::cout << "Scanning for problems" << std::endl;
}

void MenuCLI::scan_completed(const std::function<void (bool)>& callback) {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_state = NONE;
  progressBar->done();

  std::cout << "Nb duplicate songs: " << rhythmdb->nb_same_file() << std::endl;
  //std::cout << "Nb possibly duplicate songs: " << rhythmdb->nb_same_info_only() << std::endl;
  std::cout<<std::endl;

  char answer = '-';
  while (!(answer == 'Y' || answer == 'y' || answer == 'N' || answer == 'n')) {
    std::cout << "Do you want to fix detected problems? [Y/n] ";
    while (std::cin.peek() != '\n') {
      std::cin >> answer;
    }
    std::cin.ignore();
  }

  callback(answer == 'Y' || answer == 'y' || answer == ' ');
}

void MenuCLI::start_fix() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_fix_count = 0;
  progressBar = std::make_unique<ProgressBar>(rhythmdb->nb_same_file(), 70);
  current_state = FIX;

  std::cout << "Applying fixes" << std::endl;
}

void MenuCLI::fix_completed() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_state = NONE;
  progressBar->done();
  std::cout << "Done";
}

void MenuCLI::show_load() {
  static int pos = 0;
  std::cout << "Loading Rhythmbox Database " << cursor[pos] << '\r';
  std::cout.flush();
  pos = (pos + 1) % 4;
}

void MenuCLI::show_scan() {
  unsigned int scan_count = rhythmdb->scan_progress();
  unsigned int diff = scan_count - current_scan_count;
  current_scan_count = scan_count;

  *(progressBar.get()) += diff;
  progressBar->display();
}

void MenuCLI::show_fix() {
  unsigned int fix_count = rhythmdb->fix_progress();
  unsigned int diff = fix_count - current_fix_count;
  current_fix_count = fix_count;

  *(progressBar.get()) += diff;
  progressBar->display();
}
