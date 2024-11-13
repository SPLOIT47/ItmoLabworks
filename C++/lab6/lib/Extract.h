#pragma once
#include "Archive.h"
#include "Options.h"
#include "ParseTerminal.h"
#include <string>
#include <fstream>
#include <iostream>

void ExtractFromArchive(const std::string &archive_name, const std::string &output_file_name);

void CorrectErrors(std::vector<int8_t> &ControlBits, std::vector<char> &DataBlock);

void ExtractEncodedBlock(std::ifstream &archive, std::vector<int8_t> &ControlBits, std::vector<char> &DataBlock);