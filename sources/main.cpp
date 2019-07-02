#include "appcli.h"

//#include "date.h"
//#include <chrono>

int main() {
  AppCLI app_cli;
  app_cli.run();

//  //Display date from timestamp
//  using namespace date;
//  auto first_seen = std::chrono::system_clock::from_time_t(1507311790);
//  auto date = year_month_day(floor<days>(first_seen));
//  std::cout << date << ' ' << date::make_time(first_seen - date::floor<days>(first_seen)) << "\n";

  return 0;
}
