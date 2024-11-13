#pragma once
#include <cstdint>
#include <iostream>
#include <memory>
const int16_t kMaximumCordsCount = INT16_MAX;

struct Options {
  int32_t length = 0;
  int32_t width = 0;
  char* filename;
  char* path_for_images;
  int32_t max_iter;
  int32_t freq = 0;

  int16_t min_x = 0;
  int16_t max_x = 0;
  int16_t min_y = 0;
  int16_t max_y = 0;
  int16_t real_cords_count = 0;
  int16_t x_cords[kMaximumCordsCount];
  int16_t y_cords[kMaximumCordsCount];
  uint64_t height_values[kMaximumCordsCount];
  int16_t shift_zero_cord_x = 0;
  int16_t shift_zero_cord_y = 0;
  uint64_t** desk;

   void CorrectValues() {
    if (min_x < 0 && max_x >= 0) {
      width = abs(min_x) + max_x + 1;
      shift_zero_cord_x = abs(min_x);
    } else if (min_x < 0 && max_x < 0) {
      width = abs(min_x) - abs(max_x) + 1;
      shift_zero_cord_x = abs(min_x);
    } else {
      width = max_x + 1;
    }

    if (min_y < 0 && max_y >= 0) {
      length = abs(min_y) + max_y + 1;
      shift_zero_cord_y = abs(min_y);
    } else if (min_y < 0 && max_y < 0) {
      length = abs(min_y) - abs(max_y) + 1;
      shift_zero_cord_y = abs(min_y);
    } else {
      length = max_y + 1;
    }
  }
};