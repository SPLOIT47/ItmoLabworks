#include <string>
#include "Options.h"

int HamArc::FindArchive(const std::string& value) {
  for (int i = 0; i < archives_list.size(); ++i) {
    if (archives_list[i]->archive_name == value) return i;
  }
  return -1;
}