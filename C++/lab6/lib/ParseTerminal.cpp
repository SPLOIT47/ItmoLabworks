#include "Archive.h"
#include "Extract.h"
#include "Options.h"
#include "Delete.h"
#include "Merge.h"
#include <iostream>
#include <cstring>
#include <string>


void Parse(int argc, char** argv, HamArc::Options& options) {
  HamArc Ham_arc;
  int i = 1;
  while (i < argc) {
    if (strncmp(argv[i], "--create", 8) == 0) {
      HamArc::Options* new_arc = new HamArc::Options;
      new_arc->archive_name = argv[i] + 9;
      Ham_arc.archives_list.push_back(new_arc);
      ++i;
      CreateEmptyArchive(Ham_arc.archives_list.back()->archive_name);
    } else if (strcmp(argv[i], "-c") == 0) {
      HamArc::Options* new_arc = new HamArc::Options;
      new_arc->archive_name = argv[i + 1];
      Ham_arc.archives_list.push_back(new_arc);
      i += 2;
      CreateEmptyArchive(Ham_arc.archives_list.back()->archive_name);
    } else if (strncmp(argv[i], "--file", 6) == 0) {
      Ham_arc.archives_list.back()->archive_name = argv[i] + 7;
      Ham_arc.current_archive = Ham_arc.archives_list.back()->archive_name;
      ++i;
    } else if (strcmp(argv[i], "-f") == 0) {
      Ham_arc.archives_list.back()->archive_name = argv[i + 1];
      Ham_arc.current_archive = Ham_arc.archives_list.back()->archive_name;
      i += 2;
    } else if (strncmp(argv[i], "--list", 6) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      if (index == -1) {
        std::cerr << "No such archive in availiable archives";
        exit(EXIT_FAILURE);
      }
      for (std::string name : Ham_arc.archives_list[index]->file_name_list) {
        std::cout << name << "  ";
      }
      std::cout << '\n';
      ++i;
    } else if (strcmp(argv[i], "-l") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      for (std::string name : Ham_arc.archives_list[index]->file_name_list) {
        std::cout << name << "  ";
      }
      std::cout << '\n';
      ++i;
    } else if (strncmp(argv[i], "--extract", 9) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Ham_arc.archives_list[index]->file_name_list.erase(std::remove(Ham_arc.archives_list[index]->file_name_list.begin(), Ham_arc.archives_list[index]->file_name_list.end(), std::string(argv[i] + 9)), Ham_arc.archives_list[index]->file_name_list.end());
      ExtractFromArchive(Ham_arc.current_archive, argv[i] + 9);
      ++i;
    } else if (strcmp(argv[i], "-x") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Ham_arc.archives_list[index]->file_name_list.erase(std::remove(Ham_arc.archives_list[index]->file_name_list.begin(), Ham_arc.archives_list[index]->file_name_list.end(), std::string(argv[i + 1])), Ham_arc.archives_list[index]->file_name_list.end());
      ExtractFromArchive(Ham_arc.current_archive, argv[i + 1]);
      i += 2;
    } else if (strncmp(argv[i], "--append", 8) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Ham_arc.archives_list[index]->file_name_list.push_back(argv[i + 9]);
      ++i;
      AddFileToArchive(Ham_arc, argv[i] + 9);
    } else if (strcmp(argv[i], "-a") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Ham_arc.archives_list[index]->file_name_list.push_back(argv[i + 1]);
      i += 2;
      AddFileToArchive(Ham_arc, argv[i + 1]);
    } else if (strncmp(argv[i], "--delete", 8) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      RemoveFileFromArchive(Ham_arc.archives_list[index]->archive_name, argv[i] + 9);
      ++i;
    } else if (strcmp(argv[i], "-d") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      RemoveFileFromArchive(Ham_arc.archives_list[index]->archive_name, argv[i + 1]);
      i += 2;
    } else if (strncmp(argv[i], "--concatenate", 13) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Merge(Ham_arc.archives_list[index]->archive_name, argv[i] + 14);
      ++i;
    } else if (strcmp(argv[i], "-A") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Merge(Ham_arc.archives_list[index]->archive_name, argv[i + 1]);
      i += 2;
    } else if (strncmp(argv[i], "--ExtractAll", 12) == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      for (auto file : Ham_arc.archives_list[index]->file_name_list) {
        ExtractFromArchive(Ham_arc.current_archive, file);
      }
      ++i;
    } else if (strcmp(argv[i], "-l") == 0) {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      for (auto file : Ham_arc.archives_list[index]->file_name_list) {
        ExtractFromArchive(Ham_arc.current_archive, file);
      }
      ++i;
    } else {
      int index = Ham_arc.FindArchive(Ham_arc.current_archive);
      Ham_arc.archives_list[index]->file_name_list.push_back(argv[i]);
      AddFileToArchive(Ham_arc, argv[i]);
      ++i;
    }
  }
}   
