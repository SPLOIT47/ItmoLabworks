#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <cstring>
#include <filesystem>
#include <string>

class __table {
public:
  __table() = default;

  __table(std::string name, std::string file_name, size_t size, size_t block_size) 
  : _file_name(file_name), _size(10 * size), _block_size(block_size), _name(name) {
    std::ofstream temp(_file_name);
    temp.close();

    if (size < 1000) { _collision_per_file = 20; }
    else if (size < 10000) { _collision_per_file = 10; }
    else { _collision_per_file = 1; }

    str_cmp.resize(_block_size, ' ');
  }

  void handleCollision(const char* buff, size_t hash) {
        std::vector<char> view(_block_size, ' ');
        size_t position = hash * _block_size;
        for (const std::string& it : _collisions) {
            std::fstream file(it, std::ios::binary | std::ios::out | std::ios::in);
            file.seekg(position, std::ios::beg);
            file.read(view.data(), _block_size);
           
            while (strcmp(view.data(), str_cmp.data()) != 0 && strcmp(getKey(view.data()).c_str(), getKey(buff).c_str())) {
              position += _block_size;
              file.clear();
              file.seekg(position, std::ios::beg);
              file.read(view.data(), _block_size);
              if (file.eof()) {
                break;
              }
            }
            file.clear();
            file.seekp(position, std::ios::beg);
            file.write(fixSize(buff), _block_size);

            file.close();
            return;
        }


        std::string base_name = _file_name;
        base_name.pop_back();  base_name.pop_back();  base_name.pop_back(); base_name.pop_back();
        std::string call_name = base_name + "_collision" + std::to_string(_collisions.size()) + ".txt";
        _collisions.push_back(call_name);
        std::ofstream tmp(call_name);
        tmp.close();
        std::fstream new_coll(call_name, std::ios::binary | std::ios::out | std::ios::in);
        new_coll.seekp(position);
        new_coll.write(fixSize(buff), _block_size);
        new_coll.close();
    }

  bool is_collision(size_t pos, std::string key) {
    std::fstream _file_stream(_file_name, std::ios::binary | std::ios::in | std::ios::out);
    _file_stream.seekg(pos, std::ios::beg);
    char* view = new char[_block_size];
    if (!_file_stream.eof()) {
       _file_stream.read(view, _block_size);
       if (strcmp(view, "") != 0 && strncmp(view, key.c_str(), key.length())!= 0) {
        return true;
       }
    }
   
    delete[] view;
    return false;
  }
  void write(const char* key, const char* buff) {
    size_t hash = hasher(key, _size);
    size_t position = hash * _block_size;
    std::string writeble = key;
      writeble += " ";
      writeble += fixSize(buff);
    if (is_collision(position, key)) {
      handleCollision(writeble.c_str(), hash);
    } else {
      std::fstream f(_file_name, std::ios::binary | std::ios::in | std::ios::out);
      f.clear();
      f.seekp(position, std::ios::beg);
      f.write(writeble.c_str(), _block_size);
      f.close();
      if (_name == "posting_lists") {
        std::fstream file(_file_name, std::ios::binary | std::ios::in | std::ios::out);
        file.seekg(position, std::ios::beg);
        file.write(writeble.c_str(), _block_size);
      }
    } 
  }   

  std::string get(const char* key) {
    std::fstream _file_stream(_file_name, std::ios::binary | std::ios::in | std::ios::out);
    size_t hash = hasher(key, _size);
    size_t positon = hash * _block_size;
    _file_stream.seekg(positon, std::ios::beg);
    char* view = new char[_block_size];
    _file_stream.read(view, _block_size);
    std::string readed_key = getKey(view);
    if (readed_key == key) {
      std::string str = getData(view);
      return Trim(str);
    } else {
      for (auto it : _collisions) {
        std::fstream file(it, std::ios::binary | std::ios::in | std::ios::out);
        for (int8_t counter = 0; counter < _collision_per_file; ++counter, positon += _block_size) {
          file.seekg(positon, std::ios::beg);
          file.read(view, _block_size);
          if (getKey(view) == key) {
            file.close();
            std::string str = getData(view);
            return Trim(str);
          }
        } 
      }
    }
    return "";
  }

  struct hash {
    size_t operator()(const char* buff, size_t size) {
      std::string str = buff;
      const unsigned long long k = 0x9ae16a3b2f90404fULL;
      size_t h = 0, m = 1;
      for (char it : str) {
        size_t x = (size_t) (it - 'a' + 1);
        h = (h + m * x) % size;
        m = (m * k) % size;
      }
      return h;
    } 
  };

  std::string getKey(std::string buff) {
    std::string key;
    for (auto ch : buff) {
      if (ch == ' ') {
        break;
      }
      key += ch;
    } 
    
    return key;
  }

  std::string getData(std::string buff) {
    std::string data;
    int pos = 0;
    while (buff[pos] != ' ') ++pos;
    ++pos;
    std::string dt;
    for ( ; pos < buff.length(); ++pos) {
      dt += buff[pos];
    }
    return dt;
  } 

  char* fixSize(const char* buff) {
    char* writeable = new char[_block_size];
      strcpy(writeable, buff); 
      size_t writeable_length = strlen(buff); 
      while (writeable_length < _block_size) {
          writeable[writeable_length++] = ' '; 
      }
    return writeable;
  }

  std::string Trim(std::string& str) {
    for (int i = str.length() - 1; i >= 0; --i) {
      if (str[i] != ' ') break;
      str.pop_back();
    }
    return str;
  }

 private:
  hash hasher;
  std::string _name;
  size_t _size;
  size_t _block_size;
  size_t _collision_per_file;
  std::vector<char> str_cmp;
  std::string _file_name;
  std::vector<std::string> _collisions;
};