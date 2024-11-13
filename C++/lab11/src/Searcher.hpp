#pragma once
#include "DataBase/DataBase.hpp"
#include "Indexer.hpp"
#include <queue>
#include <unordered_map>

class Searcher {
private:
  DataBase* _db;

public:

  Searcher() = default;
  Searcher(DataBase* db) : _db(db) {}
  
  void Search(std::map<int64_t, double> top_k_heap, std::vector<std::string> terms) { 
    size_t max_size = 0;
    for (auto it : terms) {
      max_size = std::max(max_size, it.length());
    }
    auto vec = sortMapByValue(top_k_heap);
    for (auto did : vec) {
      std::unordered_map<std::string, int64_t> terms_counter;
      std::ifstream file(_db->get("files_path", std::to_string(did.first)));
      std::cout << "Scanning: " << _db->get("files_path", std::to_string(did.first)) << '\n';
      char ch;
      int64_t lines = 1;
      std::string current_word;
      while(file.get(ch)) {
        if (ch == '\n') {
          ++lines;
        }

        if (isalpha(ch)) {
          current_word += ch;
        } else {
          for (const auto &term : terms) {
            if (term == current_word) {
              std::cout << term << " line: " << lines << '\n';
              terms_counter[term]++;
            }
          }
          current_word.clear();
        }  
      }
      if (current_word != "") {
          for (const auto &term : terms) {
          if (term == current_word) {
            std::cout << term << " in line: " << lines << '\n';
            terms_counter[term]++;
          }
        }
      }
      std::cout << "total count of terms in file: " << _db->get("files_path", std::to_string(did.first)) << '\n';
      for (auto it : terms_counter) {
        std::cout << it.first << ": " << it.second << '\n';
      }
    }
  }

  std::vector<std::pair<int64_t, double>> sortMapByValue(const std::map<int64_t, double>& map) {
    std::vector<std::pair<int64_t, double>> vec(map.begin(), map.end());
    std::sort(vec.begin(), vec.end(), [](const std::pair<int64_t, double>& a, const std::pair<int64_t, double>& b) {
        return a.second > b.second;
    });
    return vec;
  }
};  