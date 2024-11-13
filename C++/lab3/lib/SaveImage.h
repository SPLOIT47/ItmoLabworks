#pragma once

#include <string>

#include "Options.h"

#pragma pack(push, 1)

struct BMPHEAD {
  unsigned char id[2];
  uint32_t file_size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset_pixel_array;
};

struct BMPINFOHEADER {
  uint32_t header_size;
  uint32_t width;
  uint32_t height;
  uint16_t color_planes;
  uint16_t bpp;
  uint32_t compression;
  uint32_t image_size;
  int32_t horizontal_resolution;
  int32_t vertical_resolution;
  uint32_t colors;
  uint32_t important_colors;
};

struct ColorPalette {
  unsigned char white[4] = {255, 255, 255, 0};
  unsigned char green[4] = {0, 128, 0, 0};
  unsigned char yellow[4] = {0, 255, 255, 0};
  unsigned char purple[4] = {128, 0, 128, 0};
  unsigned char black[4] = {0, 0, 0, 0};
};

#pragma pack(pop)

void SaveImage(Options& options, const char* file_path);
