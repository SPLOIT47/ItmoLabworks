#include "Parser.h"
#include <iostream>
#include <fstream>

void ParseFile(Options& options) {
  std::ifstream file(options.filename, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Failed to open file";
    exit(EXIT_FAILURE);
  }

  int len = 1;
  char simbol;
  while (file.get(simbol)) {
    if (simbol == '\n') {
      ++len;
    }
  }
  file.clear();
  file.seekg(0);
  int index = 0;

  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (index % 3 == 0) {
        file >> options.x_cords[index / 3];
        options.max_x = std::max(options.max_x, options.x_cords[index / 3]);
        options.min_x = std::min(options.min_x, options.x_cords[index / 3]);
        ++index;
      } else if (index % 3 == 1) {
        file >> options.y_cords[index / 3];
        options.max_y = std::max(options.max_y, options.y_cords[index / 3]);
        options.min_y = std::min(options.min_y, options.y_cords[index / 3]);
        ++index;
      } else if (index % 3 == 2) {
        file >> options.height_values[index / 3];
        ++index;
      }
    }
    ++options.real_cords_count;
  }
}


void ParseArguments(int argc, char* argv[], Options& options) {
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-i") && i < argc - 1) {
      options.filename = argv[i + 1];
    } else if (!strcmp(argv[i], "-o") && i < argc - 1) {
      options.path_for_images = argv[i + 1];
    } else if (!strcmp(argv[i], "-m") && i < argc - 1) {
      options.max_iter = std::atoi(argv[i  + 1]);
    } else if (!strcmp(argv[i], "-f") && i < argc - 1) {
      options.freq = std::atoi(argv[i + 1]);
    } else if (!strncmp(argv[i], "--input", 7)) {
      options.filename = new char[strlen(argv[i]) - 8];
      strcpy(options.filename, argv[i] + 8);
    } else if (!strncmp(argv[i], "--output", 8)) {
      options.path_for_images = new char[strlen(argv[i]) - 9];
      strcpy(options.path_for_images, argv[i] + 9);
    } else if (!strncmp(argv[i], "--max-iter", 10)) {
      options.max_iter = std::atoi(argv[i] + 11);
    } else if (!strncmp(argv[i], "--freq", 6)) {
      options.freq = std::atoi(argv[i] + 7);
    }
  }
}

