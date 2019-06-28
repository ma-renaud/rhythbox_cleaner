#include <iostream>
#include "mxml.h"
#include "rhythmdbxml.h"
#include "classwithcallback.h"
#include "rhythmdb.h"

#include "date.h"
#include <chrono>

void sax_cb(mxml_node_t *node,
            mxml_sax_event_t event,
            void *data) {
  std::ignore = data;
  if (event != MXML_SAX_ELEMENT_CLOSE)
    mxmlRetain(node);
}

mxml_type_t
type_cb(mxml_node_t *node)
{
  const char *type;
/*
* You can lookup attributes and/or use the
* element name, hierarchy, etc...
*/

  type = node->value.element.name;
  if (!strcmp(type, "x") || !strcmp(type, "y") || !strcmp(type, "w") || !strcmp(type, "h"))
    return (MXML_INTEGER);
  else
    return (MXML_TEXT);
}

int main() {
  RhythmDB rhythmdb;
  RhythmDBXML parser(&rhythmdb);

  FILE *fp;
  mxml_node_t *tree;

  fp = fopen("../rhythmdb.xml", "r");
  tree = mxmlSAXLoadFile(nullptr, fp, MXML_TEXT_CALLBACK, SaxMemberFunctionCallback(&parser, &RhythmDBXML::sax_callback), nullptr);
  fclose(fp);

  parser.write_to_file("../filename.xml");

  std::cout << "Nb songs: " << rhythmdb.nb_songs() << std::endl;
  std::cout << "Nb radios: " << rhythmdb.nb_radios() << std::endl;
  std::cout << "Nb ignores: " << rhythmdb.nb_ignores() << std::endl;
  std::cout << "Nb podcast feeds: " << rhythmdb.nb_podcast_feeds() << std::endl;
  std::cout << "Nb podcast posts: " << rhythmdb.nb_podcast_posts() << std::endl;
  std::cout<<std::endl;



  std::cout<<std::endl;

  //Display date from timestamp
  using namespace date;
  auto first_seen = std::chrono::system_clock::from_time_t(1507311790);
  auto date = year_month_day(floor<days>(first_seen));
  std::cout << date << ' ' << date::make_time(first_seen - date::floor<days>(first_seen)) << "\n";

  std::ignore = tree;
  return 0;
}
