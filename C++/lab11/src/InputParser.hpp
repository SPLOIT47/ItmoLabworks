#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "structures/__lt.hpp"
#include "DataBase/DataBase.hpp"

class InputParser {
public:
  InputParser(DataBase* db, __lt* lt) : files_count(0), _db(db), _tree(lt) {}
  
  void ParseHeader(int argc, char** argv, bool flag);
  void ParseCommand(const std::string& cmd);
  bool isCorrect(std::string str);
  size_t size();
  size_t longest();
  size_t longestTerm();
  size_t termsCount();
private:
  __lt* _tree;
  DataBase* _db;
  size_t files_count;
  size_t terms_count;
  size_t longest_path;
  size_t longest_term;
  
  void DirrectoryTraversal(const std::filesystem::path& path, bool flag);
};