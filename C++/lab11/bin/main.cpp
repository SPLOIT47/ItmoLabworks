#include "src/InputParser.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include "fstream"
#include "src/Indexer.hpp"
#include "src/DataBase/__table.hpp"
#include "src/SimpleSearchEngine.hpp"

struct hash {
    size_t operator()(const char* buff, size_t size) {
      std::string str = buff;
      const unsigned long long k = 0x9ae16a3b2f90404fULL;
      size_t h = 0, m = 1;
      for (char it : str) {
        size_t x = (size_t) (it - 'a' + 1);
        h = (h + m * x) % 100;
        m = (m * k) % 100;
      }
      return h;
    } 
  };

int main(int argc, char** argv) {
  // InputParser parser;
  // parser.ParseHeader(argc, argv);
  // std::vector<__file> vec = parser.GetFilePaths();
  // for (auto it : vec) {
  //   std::cout << it.name() << '\n' << it.path() << '\n' << it.id() << '\n';
  // }
  // std::cout << '\n';
  // std::string cmd;
  // std::getline(std::cin, cmd);
  // parser.ParseCommand(cmd);
  // std::cout << '\n';
  // auto tree = parser.lt();
  // auto arr = tree.TermList();
  // for(auto it : arr) {
  //   std::cout << it << '\n';
  // }
  // Indexer index;

  // __lt tree;
  // std::string str = "biba OR boba";
  // tree.createAST(str);
  // //tree.traverseTree(tree.root());
  // std::vector<std::string> t = tree.TermList();
  // __table t("/Users/alex/labwork11-SPLOIT47/src/DataBase/test.txt");
  // std::function<char*(int, int)> f = summ;
  // t.write(f, 1, 2);
  SSE sse("/Users/alex/labwork11-SPLOIT47");
  sse.init(argc, argv);
  sse.run();
}     

