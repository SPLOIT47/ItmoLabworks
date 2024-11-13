#include "Indexer.hpp"


std::string getLast(const std::string& dids) {
  std::string last;
  for (int i = dids.length() - 1; dids[i] != ' ' && i >= 0; --i) {
    last += dids[i];
  }
  std::reverse(last.begin(), last.end());
  return last;
}

void Indexer::GeneratePostingLists(int64_t max_did) {
  std::vector<std::string> terms = _tree->TermList();
  for (int i = 0; i < max_did; ++i) {
    std::string path = _db->get("files_path", std::to_string(i));
    std::ifstream file(path);
    if (!file.is_open() || file.bad()) {
      std::cerr << "Couldn't open: " << path << '\n';
      continue;
    }
    std::string word;
    while (file >> word) {
      for (auto it : terms) {
        if (it == word) {
          std::string dids = _db->get("posting_lists", it);
          if (dids == "") {
              dids += std::to_string(i);
              _db->Add("posting_lists", it, dids);
          } else {
            if (getLast(dids) != std::to_string(i)) {
              dids += " ";
              dids += std::to_string(i);
              _db->Add("posting_lists", it, dids);
            }
          }
          break;
        }
      }
    }
  }
}

std::map<int64_t, double> Indexer::queue() {
  return top_k_heap;
}

void Indexer::calculateScores() {
  std::vector<std::string> terms = _tree->TermList();
  std::vector<std::pair<int64_t, std::string>> iterators;
  std::unordered_map<std::string, int64_t> iterators_indexes;
  for (auto it : terms) {
    iterators.push_back({max_did + 1, it});
  }
  for (int64_t i = 0; i < terms.size(); ++i) {
    std::string dids = _db->get("posting_lists", terms[i]);
    std::vector<int64_t> vec = parseInt(dids);
    if (vec.size() > 0) {
      iterators[i].first = vec[0];
    }
  }

  bool exit_flag = false;
  for (; ;) {
    std::sort(iterators.begin(), iterators.end());
    std::unordered_map<std::string, double> map;
    int64_t index = 0;
      
    for (; index < iterators.size(); ++index) {
      if (index > 0  && iterators[index - 1].first != iterators[index].first) {

        break;
      }
      double score = evaluate(iterators[index].first, iterators[index].second);
      map.insert({iterators[index].second, score});
      ++iterators_indexes[iterators[index].second];
      _tree->setCurrDid(iterators[index].first);
    }
    _tree->refreshScores(_tree->root());
    _tree->setScores(_tree->root(), map);
    double res_score = _tree->getResultScore();
    if(res_score > 0) { 
      top_k_heap.insert({_tree->currDid(), res_score}); 
    }

    for (auto& it : iterators) {
      if (it.first != max_did + 1) {
        std::string str = _db->get("posting_lists", it.second);
        std::vector<int64_t> vec = parseInt(str);
        if (iterators_indexes[it.second] >= vec.size()) {
          exit_flag = true;
          break;
        }
        it.first = vec[iterators_indexes[it.second]];
      }
    }
    if (exit_flag) { break; }
  }
}

double Indexer::evaluate(int64_t did, std::string term) { 
  std::string name = _db->get("files_path", std::to_string(did));
  std::ifstream file(name);
  std::string word;
  double term_count = 0;
  double total_count = 0;
  while (file >> word) {
    if (word == term) {
      ++term_count;
    }
    ++total_count;
  }

  
  double tf = term_count / total_count;
  std::string str = _db->get("posting_lists", term);
  int64_t size = parseInt(str).size();
  double div = ((double) max_did) / ((double)size);
  double idf = log(div + 1);
  return tf * idf;
}

std::vector<int64_t> Indexer::parseInt(const std::string& str) {
  std::vector<int64_t> nums;
  std::stringstream ss(str);
  int64_t num;
  while (ss >> num) {
    nums.push_back(num);
    if (ss.peek() == ' ') ss.ignore();
  }
  return nums;
}

void Indexer::setMaxDid(int64_t did) {
  max_did = did;
}

void Indexer::clear() {
  top_k_heap.clear();
}
