#pragma once
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include "structures/__lt.hpp"
#include "DataBase/DataBase.hpp"

struct Comparator {
  bool operator()(std::pair<int64_t, double> p1, std::pair<int64_t, double> p2) const { 
    return p1.second < p2.second;
  }
};

class Indexer {
private:
  std::map<int64_t, double> top_k_heap;
  int64_t max_did;
  DataBase* _db;
  __lt* _tree;

public:
  Indexer(DataBase* db, __lt* lt) : _db(db), _tree(lt) {}
  void GeneratePostingLists(int64_t max_did);
  std::map<int64_t, double> queue();

  void calculateScores();
  double evaluate(int64_t did, std::string term);
  std::vector<int64_t> parseInt(const std::string& str);
  void setMaxDid(int64_t did);
  void clear();
};