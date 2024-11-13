#include "Archive.h"
#include "Options.h"
#include "ParseTerminal.h"
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>

void reverse(std::bitset<8>& data) {
  for (int i = 0; i < 4; ++i) {
    std::swap(data[i], data[7 - i]);
  }
}

bool CorrectError(std::bitset<8>& data, const std::bitset<4>& control_bits) {
    int8_t error_position = 0;
    int8_t new_result_bit = data[2] ^ data[4] ^ data[6];
    if (new_result_bit != control_bits[0]) error_position += 1;
     new_result_bit = data[2] ^ data[5] ^ data[6];
    if (new_result_bit != control_bits[1]) error_position += 2;
     new_result_bit = data[4] ^ data[5] ^ data[6];
    if (new_result_bit != control_bits[2]) error_position += 4;

    if (error_position != 0) {
      data[error_position] = !data[error_position];
    }

    int32_t control_sum = data[2] ^ data[4] ^ data[5] ^ data[6];
    if (control_sum != control_bits[3]) {
      return false;
    }
    return true;
}

bool ReadEncodedBlock(std::ifstream &archive, std::bitset<4>& control_bits, char& data) {
  int8_t bit_position = 0;
  int8_t total_bits = 8;
  
  char buffer;
  for (int j = 0; j < 2; ++j) {

    int8_t control_bit_position = 1;
    int8_t control_bit_position_in_array = 0;
    buffer = archive.get();
    std::bitset<8> bits1(buffer);
    reverse(bits1);
    for (int i = 1; i <= total_bits; ++i) {
      if (control_bit_position == i) {
        control_bits[control_bit_position_in_array] = bits1[i - 1];
        ++control_bit_position_in_array;
        control_bit_position *= 2;
      } else {
        data |= ((bits1[i - 1]) << bit_position);
        ++bit_position;
      }
    }
    if (!CorrectError(bits1, control_bits)) {
      return false;
    }
  }
  return true;
}

void ExtractFromArchive(const std::string &archive_name, const std::string &output_file_name) {
  std::ifstream archive(archive_name + ".haf", std::ios::binary);
  if (!archive.is_open()) {
    std::cerr << "Failed to open archive: " << archive_name << '\n';
    return;
  }

  int blockSize = 8;
  FileHeader header;
  archive.seekg(0, std::ios::beg);

  bool is_archive_include_file = false;

  while (archive.peek() != EOF) {
    archive.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));

    if (archive.gcount() < sizeof(FileHeader)) {
      std::cerr << "Error reading header from archive.\n";
      break;
    }

    std::string archive_filename(header.filename);

    if (archive_filename == output_file_name) {
      is_archive_include_file = true;
      break;
    }

    archive.ignore(header.filesize);
  }


  if (!is_archive_include_file) {
    std::cerr << "File " << output_file_name << " not found in archive.\n";
    return;
  }

  size_t last_dot_index = output_file_name.find_last_of(".");
  std::string base_name = output_file_name.substr(0, last_dot_index);
  std::string extension = output_file_name.substr(last_dot_index);

  std::string new_file_name = base_name + "_extracted" + extension;
  std::ofstream output_file(new_file_name, std::ios::binary);

  if (!output_file.is_open()) {
    std::cerr << "Failed to create file: " << output_file_name << '\n';
    return;
  }

  int32_t remaining_file_size = header.filesize;
  int32_t block_size_in_bytes = 1;
  std::bitset<4> control_bits;
  char data = 0;
  while (remaining_file_size > 0 && archive.peek() != EOF) {
    data = 0;
    if (ReadEncodedBlock(archive, control_bits, data)) {
      output_file.write(&data, block_size_in_bytes);
      remaining_file_size -= 2 * block_size_in_bytes;
    } else {
      std::cerr << "In File: " << output_file_name << " detected 2 errors! Extracting has been stopped.";
      break;
    }
  }

  std::cout << "File: " << output_file_name << " has been extracted" << '\n';
  archive.close();
  output_file.close();
}      

