#include "InputParser.hpp"
#include "DataBase/DataBase.hpp"
#include "Indexer.hpp"
#include "structures/__lt.hpp"
#include "Searcher.hpp"
class SSE {
private:
  InputParser* _input_parser;
  Indexer* _indexer;
  DataBase* _db;
  Searcher* _searcher;
  __lt* tree;
public:
  
  SSE(std::string path) {
    _db = new DataBase(path);
    tree = new __lt;
    _input_parser = new InputParser(_db, tree);
    _indexer = new Indexer(_db, tree);
    _searcher = new Searcher(_db);
  }

  void init(int argc, char** argv) {
    _input_parser->ParseHeader(argc, argv, 0);

    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };

    _db->AddTable("files_path", _input_parser->longest() + rankCount(_input_parser->size()) + 1, _input_parser->size());
    _input_parser->ParseHeader(argc, argv, 1);
    _indexer->setMaxDid(_input_parser->size());
  }

  void run() {
    std::string input;
    while (std::getline(std::cin, input)) {
      if (input == ":q") {
        return;
      }
      if (input == "") continue;
      tree->clear();
      _indexer->clear();
      _input_parser->ParseCommand(input);
      _db->AddTable("posting_lists", _input_parser->longestTerm() + 1 + 2*_input_parser->size(), _input_parser->termsCount());
      _indexer->GeneratePostingLists(_input_parser->size());
      _indexer->calculateScores();
      for (auto it : _indexer->queue()) {
        std::cout << it.first << ' ' << it.second << '\n';
      }
      std::cout << '\n';
      _searcher->Search(_indexer->queue(), tree->TermList());
      std::cout << '\n';
    }
  }
};