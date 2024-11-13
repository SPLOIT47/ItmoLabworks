#include "Archive.h"
#include <fstream>
#include <iostream>

void RemoveFileFromArchive(const std::string& archive_name, const std::string& file_to_remove) {
  std::string tempArchiveName = archive_name + ".temp";
  std::ifstream archive(archive_name + ".haf", std::ios::binary | std::ios::ate);
  std::ofstream tempArchive(tempArchiveName, std::ios::binary);

  if (!archive.is_open()) {
    std::cerr << "Failed to open archive: " << archive_name << '\n';
    return;
  }

  if (!tempArchive.is_open()) {
    std::cerr << "Failed to create temporary archive file.\n";
    return;
  }

  size_t fileSize = archive.tellg();
  archive.seekg(0, std::ios::beg);

  FileHeader header;
  bool fileFound = false;
  while (archive.tellg() < fileSize) {
    archive.read(reinterpret_cast<char*>(&header), sizeof(header));
    std::streamsize bytesRead = archive.gcount();

    if (bytesRead == 0 || archive.eof()) break;

    std::vector<char> buffer(header.filesize);

    if (std::string(header.filename) == file_to_remove) {
      archive.seekg(header.filesize, std::ios::cur);
      fileFound = true;
    } else {
      tempArchive.write(reinterpret_cast<char*>(&header), sizeof(FileHeader));
      archive.read(buffer.data(), header.filesize);
      tempArchive.write(buffer.data(), header.filesize);
    }
  }

  archive.close();
  tempArchive.close();

  if (!fileFound) {
    std::cerr << "File " << file_to_remove << " not found in archive.\n";
    std::remove(tempArchiveName.c_str());
    return;
  }

  std::remove((archive_name + ".haf").c_str());
  std::rename(tempArchiveName.c_str(), (archive_name + ".haf").c_str());

  std::cout << "File " << file_to_remove << " has been removed from archive.\n";
}
