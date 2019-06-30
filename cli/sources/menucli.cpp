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

  int nb_songs = rhythmdb->nb_songs() - 1;
  unsigned int total = ((nb_songs * nb_songs) + nb_songs) / 2;

  progressBar = std::make_unique<ProgressBar>(total, 70);
  current_state = SCAN;

  std::cout << "Scanning for problems" << std::endl;
}

void MenuCLI::scan_completed() {
  std::lock_guard<std::mutex> lockGuard(mutex);
  current_state = NONE;
  progressBar->done();

  std::cout << "Nb duplicate songs: " << rhythmdb->nb_same_file() << std::endl;
  std::cout << "Nb possibly duplicate songs: " << rhythmdb->nb_same_info_only() << std::endl;
  std::cout<<std::endl;

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
