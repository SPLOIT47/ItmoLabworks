#include "SandModel.h"
#include "SaveImage.h"

#include <filesystem>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Desk.h"


void IntToChar(uint32_t number, char* char_array) {
    if (number == 0) {
        char_array[0] = '0';
        char_array[1] = '\0';
        return;
    }
    
    int numDigits = 0;
    int temp = number;
    while (temp != 0) {
        temp /= 10;
        numDigits++;
    }
  
    int index = numDigits - 1;
    while (number != 0) {
        int digit = number % 10;
        char_array[index] = '0' + digit;
        number /= 10;
        index--;
    }
    char_array[numDigits] = '\0'; 
}

void SandModel(Options& options) {
  int32_t z = 1;

  while (z <= options.max_iter || options.max_iter == 0) {
    bool changes = false;
    for (int16_t i = 0; i < options.length; ++i) {
      for (int16_t j = 0; j < options.width; ++j) {
        if (options.desk[i][j] >= 4) {
          if (options.desk[i][j] + 1 < 0) { // Check for overflow
            std::cout << "Overflow detected. Please check your input data or iteration count.\n";
            exit(EXIT_FAILURE);
          }
          changes = true;
          bool is_up = false;
          bool is_left = false;
          if (i == 0) is_up = true;
          if (i == options.length - 1) ExtensionDown(options);
          if (j == 0) is_left = true;
          if (j == options.width - 1) ExtensionRight(options);

          if (is_up) ExtensionUp(options), ++i;
          if (is_left) ExtensionLeft(options), ++j;
          options.desk[i][j] -= 4;
          if (i > 0) options.desk[i - 1][j] += 1;
          if (i < options.length - 1) options.desk[i + 1][j] += 1;
          if (j > 0) options.desk[i][j - 1] += 1;
          if (j < options.width - 1) options.desk[i][j + 1] += 1;
        }
      }
    }

    if (!changes) {
      if (options.max_iter == 0) {
        std::filesystem::create_directory(options.path_for_images);
        char* file_path = new char[strlen(options.path_for_images) + 26];
        strcpy(file_path, options.path_for_images);
        strcat(file_path, "/iteration_");
        char buff[11];
        IntToChar(z + 1, buff);
        strcat(file_path, buff);
        strcat(file_path, ".bmp");
        SaveImage(options, file_path);
        delete[] file_path;
      }
      std::cout << "A stable position has been achieved";
      exit(EXIT_SUCCESS);
    }

    if (options.freq != 0) {
      if (z % options.freq == 0 && z < options.max_iter) {
        std::filesystem::create_directory(options.path_for_images);
         char* file_path = new char[strlen(options.path_for_images) + 26];
        strcpy(file_path, options.path_for_images);
        strcat(file_path, "/iteration_");
        char buff[11];
        IntToChar(z + 1, buff);
        strcat(file_path, buff);
        strcat(file_path, ".bmp");
        SaveImage(options, file_path);
        delete[] file_path;
      }
    } else if (z == options.max_iter - 1 || !changes) {
      std::filesystem::create_directory(std::string(options.path_for_images));
       char* file_path = new char[strlen(options.path_for_images) + 26];
        strcpy(file_path, options.path_for_images);
        strcat(file_path, "/iteration_");
        char buff[11];
        IntToChar(z + 1, buff);
        strcat(file_path, buff);
        strcat(file_path, ".bmp");
        SaveImage(options, file_path);
        delete[] file_path;
    }

    ++z;
  }
}
