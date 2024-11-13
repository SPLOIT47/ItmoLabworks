#include "InputParser.hpp"


void InputParser::ParseHeader(int argc, char** argv, bool flag) {
  if (argc < 2) {
    std::cerr << "Wrong arguments";
    exit(0);
  }
  std::filesystem::path dir_path(argv[1]);

  if (!std::filesystem::exists(dir_path)) {
    std::cerr << "Wrong path";
    exit(0);
  }

  if (!std::filesystem::is_directory(dir_path)) {
    if (flag) {
      _db->Add("files_path", std::to_string(files_count), argv[1]);
    } else {
      ++files_count;
    }
    return;
  }

  DirrectoryTraversal(dir_path, flag);
}

void InputParser::DirrectoryTraversal(const std::filesystem::path& path, bool flag) {
  size_t last_file_index = 0;
  for (const auto& it : std::filesystem::recursive_directory_iterator(path)) {
    if (std::filesystem::is_regular_file(it)) {
      if (flag) {
        _db->Add("files_path", std::to_string(last_file_index++), it.path().string());
        std::cout << "file: " << it.path().string() << " indexed.\n";
      } else {
        ++files_count;
        longest_path = std::max(longest_path,it.path().string().length());
      }
    }
  }
}

void InputParser::ParseCommand(const std::string& cmd) {
  if (!isCorrect(cmd)) {
    std::cerr << "Wrong request";
    exit(0);
  }
  _tree->createAST(cmd);
  auto vec = _tree->TermList();
  longest_term = 0;
  for (auto it : vec) {
    longest_term = std::max(longest_term, it.length());
  }
  terms_count = vec.size();
}

bool InputParser::isCorrect(std::string str) { 
  std::stringstream ss(str);
  std::string word;
  size_t counter = 0;
  while (ss >> word) {
    if (counter++ % 3 == 1) {
      if (word != "OR" && word != "AND") {
        return false;
      }
    }
  }
  return true;
}

size_t InputParser::size() {
    return files_count;
}

size_t InputParser::longest() { return longest_path; }

size_t InputParser::longestTerm() { return longest_term; }

size_t InputParser::termsCount() { return terms_count; }
