#pragma once
#include <vector>
#include <string>

class HamArc {
public:
  struct Options {
    std::string archive_name;
    std::vector<std::string> file_name_list;
  };

  std::string current_archive;
  std::vector<Options*> archives_list;

  int FindArchive(const std::string& value);
};