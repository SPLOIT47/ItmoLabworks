#include "SaveImage.h"

#include <fstream>
#include <iostream>

#include "Options.h"

void SaveImage(Options& options, const char* file_path) {
  std::ofstream file(file_path, std::ios::out | std::ios::binary);

  if (!file) {
    std::cout << "Error creating file!" << '\n';
    exit(EXIT_FAILURE);
  }
  uint8_t padding_size = ((4 - ((options.width + 1) / 2) % 4) % 4);
  uint32_t image_width = options.width;
  uint32_t image_height = options.length;
  uint32_t file_size = sizeof(BMPHEAD) + sizeof(BMPINFOHEADER) + sizeof(ColorPalette) + options.width * options.length * 3 +
                       padding_size * options.length + 20;

  BMPHEAD header;
  header.id[0] = 'B';
  header.id[1] = 'M';
  header.file_size = file_size;
  header.reserved1 = 0;
  header.reserved2 = 0;
  header.offset_pixel_array = sizeof(BMPHEAD) + sizeof(BMPINFOHEADER) + sizeof(ColorPalette);

  BMPINFOHEADER info_header;
  info_header.header_size = sizeof(BMPINFOHEADER);
  info_header.width = image_width;
  info_header.height = image_height;
  info_header.color_planes = 1;
  info_header.bpp = 4;
  info_header.compression = 0;
  info_header.image_size = 0;
  info_header.horizontal_resolution = 0;
  info_header.vertical_resolution = 0;
  info_header.colors = 5;
  info_header.important_colors = 0;

  ColorPalette color_palette;
  file.write(reinterpret_cast<const char*>(&header), sizeof(BMPHEAD));
  file.write(reinterpret_cast<const char*>(&info_header), sizeof(BMPINFOHEADER));
  file.write(reinterpret_cast<const char*>(&color_palette), sizeof(ColorPalette));

  char padding[1] = {0};
  if (image_width % 2 == 0) {
    for (int i = 0; i < image_height; ++i) {
      for (int j = 0; j < image_width; j += 2) {
        uint8_t color1 = (options.desk[i][j] > 3 ? 4 : options.desk[i][j]);
        uint8_t color2 = (options.desk[i][j + 1] > 3 ? 4 : options.desk[i][j + 1]);
        uint8_t byte = (color1 << 4) + color2;
        file.put(byte);
      }
      file.write(padding, padding_size);
    }
  } else {
    for (int i = 0; i < image_height; ++i) {
      for (int j = 0; j < image_width - 1; j += 2) {
        uint8_t color1 = (options.desk[i][j] > 3 ? 4 : options.desk[i][j]);
        uint8_t color2 = (options.desk[i][j + 1] > 3 ? 4 : options.desk[i][j + 1]);
        uint8_t byte = (color1 << 4) + color2;
        file.put(byte);
      }
      uint8_t lbyte = (options.desk[i][image_width - 1] > 3 ? 4 : options.desk[i][image_width - 1]) << 4;
      file.put(lbyte);
      file.write(padding, padding_size);
    }
  }
}
