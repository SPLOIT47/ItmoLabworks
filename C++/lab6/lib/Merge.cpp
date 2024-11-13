#include <string>
#include <fstream>
#include <iostream>

void Merge(const std::string& archive_name1, const std::string& archive_name2) {
  std::ofstream archive1(archive_name1, std::ios::binary | std::ios::app);
  if (!archive1.is_open()) {
    std::cerr << "Failed to open archive: " << archive_name1 << '\n';
    return;
  }

  std::ifstream archive2(archive_name2, std::ios::binary);
  if (!archive2.is_open()) {
    std::cerr << "Failed to open archive: " << archive_name2 << '\n';
    return;
  }

  archive1 << archive2.rdbuf();

  archive1.close();
  archive2.close();
}
