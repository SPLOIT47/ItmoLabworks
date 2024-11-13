#pragma once
#include "Options.h"
#include "ParseTerminal.h"
#include <string>
#include <bitset>

struct FileHeader {
    char filename[1024];
    size_t filesize;
};

void UpdateControlBits(std::bitset<4>& control_bits, std::bitset<4>& data_block);

void UpdataDataBlock(std::bitset<4>& data_block, const std::vector<char>& char_data_block, const bool flag);

void WriteEncodedBlock(std::ofstream &archive, const std::bitset<4> &control_bits, const std::bitset<4> &data_block);

void WriteFileToArchive(const std::string &file_name, std::ofstream &archive, int index, HamArc& Ham_arc);

void AddFileToArchive(HamArc& Ham_arc, const std::string& file_name);

void CreateEmptyArchive(const std::string& archive_name);
