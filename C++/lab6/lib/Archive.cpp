#include "Extract.h"
#include "Options.h"
#include "ParseTerminal.h"
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>

void UpdateControlBits(std::bitset<4>& control_bits, std::bitset<4>& data_block) {
  control_bits[0] = data_block[0] ^ data_block[1] ^ data_block[2];
  control_bits[1] = data_block[0] ^ data_block[2] ^ data_block[3];
  control_bits[2] = data_block[1] ^ data_block[2] ^ data_block[3];
  int8_t control_sum = 0;
  for (int i = 0; i < 4; ++i) {
    control_sum ^= data_block[i];
  }
  control_bits[3] = control_sum;
}

void UpdataDataBlock(std::bitset<4>& data_block, const std::vector<char>& char_data_block, const bool flag) {
  int8_t shift;
  shift = flag ? 4 : 0;
  for (int i = 0; i < data_block.size(); ++i) {
    data_block[i] = ((char_data_block[i / 8] >> (i + shift % 8)) & 1);
  }
}

void WriteEncodedBlock(std::ofstream &archive, const std::bitset<4> &control_bits, const std::bitset<4> &data_block) {
  int32_t total_size = 8;
  int current_control_bit_position = 1;
  int control_bit_position_in_array = 0;
  int data_bit_position_in_array = 0;
  int bit_position = 0;
  char buffer = 0;
  int buffer_index = 0;

  for (int i = 1; i <= total_size; ++i) {
    if (i == current_control_bit_position) {
      buffer |= (control_bits[control_bit_position_in_array] << (7 - buffer_index));
      ++control_bit_position_in_array;
      current_control_bit_position *= 2;
    } else {
      buffer |= (data_block[data_bit_position_in_array] << (7 - buffer_index));
      ++data_bit_position_in_array;
    }

    ++buffer_index;

    if (buffer_index == 8) {
      archive.put(buffer);
      buffer = 0;
      buffer_index = 0;
    }
  }

  if (buffer_index > 0) {
    archive.put(buffer);
  }
}

void WriteFileToArchive(const std::string &file_name, std::ofstream &archive, int index, HamArc& Ham_arc) {
  std::ifstream file(file_name, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << file_name << '\n';
    return;
  }

  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  file.seekg(0, std::ios::beg);

  const int8_t dataBlockSize = 4;
  const int8_t blockSizeInBytes = 1;
  const int8_t ControlBitsCount = 4;
  

  FileHeader header;
  strncpy(header.filename, file_name.c_str(), sizeof(header.filename));
  header.filename[sizeof(header.filename) - 1] = '\0';
  header.filesize = static_cast<int32_t>(size * 2);
  archive.write(reinterpret_cast<char*>(&header), sizeof(FileHeader));

  std::vector<char> char_data_block(blockSizeInBytes, 0);
  std::bitset<4> data_block;
  std::bitset<4> control_bits;

  while (file) {
    file.read(&char_data_block[0], blockSizeInBytes);
    if (file.gcount() == 0) break;
    for(int i = 0; i < 2; ++i) {
      bool flag = (i == 1);
      UpdataDataBlock(data_block, char_data_block, flag);
      UpdateControlBits(control_bits, data_block);
      WriteEncodedBlock(archive, control_bits, data_block);
    }
    std::fill(char_data_block.begin(), char_data_block.end(), 0);
  }
  std::cout << "File: " << file_name << " has been archivated." << '\n';
  file.close();
}

void AddFileToArchive(HamArc& Ham_arc, const std::string& file_name) {
  std::ofstream archive(Ham_arc.current_archive + ".haf", std::ios::binary | std::ios::app);
  int index = Ham_arc.FindArchive(Ham_arc.current_archive);
  WriteFileToArchive(file_name, archive, index, Ham_arc);
}

void CreateEmptyArchive(const std::string& archive_name) {
  std::ofstream archive(archive_name + ".haf", std::ios::binary);
  if (!archive.is_open()) {
    std::cerr << "Failed to create archive: " << archive_name << '\n';
    return;
  }
}