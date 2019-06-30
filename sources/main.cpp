#include <iostream>
#include "mxml.h"
#include "rhythmdbxml.h"
#include "classwithcallback.h"
#include "rhythmdb.h"

#include <thread>
#include <functional>
#include "menucli.h"
#include "displaythread.h"

//#include "date.h"
//#include <chrono>

int main() {
  RhythmDB rhythmdb;
  RhythmDBXML parser(&rhythmdb);
  MenuCLI menu(&rhythmdb);

  DisplayThread display(&menu);
  auto display_run = std::bind(&DisplayThread::run, &display);
  std::thread threadObj(display_run);

  FILE *fp;
  fp = fopen("../rhythmdb.xml", "r");

  if (fp) {
    menu.start_load();

    mxmlSAXLoadFile(nullptr,
                    fp,
                    MXML_TEXT_CALLBACK,
                    SaxMemberFunctionCallback(&parser, &RhythmDBXML::sax_callback),
                    nullptr);
    fclose(fp);

    menu.load_completed();

    rhythmdb.search_problems();

    menu.scan_completed();

    parser.write_to_file("../filename.xml");
  }

//  //Display date from timestamp
//  using namespace date;
//  auto first_seen = std::chrono::system_clock::from_time_t(1507311790);
//  auto date = year_month_day(floor<days>(first_seen));
//  std::cout << date << ' ' << date::make_time(first_seen - date::floor<days>(first_seen)) << "\n";


  display.stop();
  if (threadObj.joinable())
    threadObj.join();
  return 0;
}
