#pragma once
#include "unordered_map"
#include "__table.hpp"
#include <cstring>
#include <filesystem>

class DataBase {
public:
  DataBase() = delete;
  DataBase(std::string path) : _data_path(path) {
    std::string p = _data_path + "/.database";
    std::filesystem::create_directories(p);
  }
  
  __table& operator[](std::string key) {
    return *_tables[key];
  }

  void AddTable(std::string name, size_t block_size, size_t avg_size) {
    std::string path = _data_path + "/.database/" + name + ".txt";
    __table* table = new __table(name, path, avg_size, block_size);
    _tables.insert({name, table});
  }

  void Add(std::string table_name, std::string key, std::string module) {
    _tables[table_name]->write(key.c_str(), module.c_str());
  }

  std::string get(std::string table_name, std::string key) {
    return _tables[table_name]->get(key.c_str());
  }

private: 
  std::unordered_map<std::string, __table*> _tables;
  std::string _data_path;
};